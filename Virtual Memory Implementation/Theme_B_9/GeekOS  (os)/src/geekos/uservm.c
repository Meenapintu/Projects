/*
 * Paging-based user mode implementation
 * Copyright (c) 2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * $Revision: 1.51 $
 * 
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "COPYING".
 */

#define GEEKOS

#include <geekos/int.h>
#include <geekos/mem.h>
#include <geekos/paging.h>
#include <geekos/malloc.h>
#include <geekos/string.h>
#include <geekos/argblock.h>
#include <geekos/kthread.h>
#include <geekos/range.h>
#include <geekos/vfs.h>
#include <geekos/user.h>
#include <geekos/projects.h>



int userDebug = 0;
#define Debug(args...) if (userDebug) Print("uservm: " args)

/* ----------------------------------------------------------------------
 * Private functions
 * ---------------------------------------------------------------------- */

// TODO: Add private functions

pte_t * Get_Table_Entry(pde_t * dir, int i, int j, bool create, bool user) {
    if(i < 512) {
        dir = kernelDir;
    }
    if(create && dir[i].present == 0) {
        pte_t * table = Alloc_Pageable_Page(&(dir[i]), 0);
        memset(table, 0, PAGE_SIZE);
        dir[i].present = 1;
        dir[i].pageTableBaseAddr = (ulong_t)table >> 12;
        dir[i].flags = VM_READ | VM_WRITE;
        if(user) {
            dir[i].flags |= VM_USER;
        }
        return &(table[j]);
    }
    if(dir[i].present != 0) {
        pte_t * table = dir[i].pageTableBaseAddr << 12;
        return &(table[j]);
    }
    return NULL;
}

void *User_To_Kernel(struct User_Context *userContext, ulong_t userPtr) {
    uchar_t *userBase = USER_VM_START;

    return (void *)(userBase + userPtr);
}

static struct User_Context *Create_User_Context() {
    struct User_Context *context;
    int index;
    
    Disable_Interrupts();
    context = (struct User_Context *)Malloc(sizeof(*context));
    if (context != 0) {
        memset(context, 0, sizeof(struct User_Context));
        context->memory = USER_VM_START;
        context->size = USER_VM_SIZE;
    }
    Enable_Interrupts();
    
    if (context == 0) {
        goto fail;
    }
    
    // Create user directory
    pde_t * dir = Alloc_Page();
    memset(dir, 0, PAGE_SIZE);
    if(dir == 0) {
        goto fail;
    }
    
    // Copy kernel 2GB
    memcpy(dir, kernelDir, 512 * sizeof(pde_t));
    context->pageDir = dir;

    context->ldtDescriptor = Allocate_Segment_Descriptor();
    if (context->ldtDescriptor == 0)
        goto fail;
    Debug("Allocated descriptor %d for LDT\n",
              Get_Descriptor_Index(context->ldtDescriptor));
    
    
    Init_LDT_Descriptor(context->ldtDescriptor, context->ldt,
                        NUM_USER_LDT_ENTRIES);
    index = Get_Descriptor_Index(context->ldtDescriptor);
    context->ldtSelector = Selector(KERNEL_PRIVILEGE, true, index);

    /* Initialize code and data segments within the LDT */
    Init_Code_Segment_Descriptor(&context->ldt[0],
                                 (ulong_t) context->memory,
                                 context->size / PAGE_SIZE, USER_PRIVILEGE);
    Init_Data_Segment_Descriptor(&context->ldt[1],
                                 (ulong_t) context->memory,
                                 context->size / PAGE_SIZE, USER_PRIVILEGE);
    context->csSelector = Selector(USER_PRIVILEGE, false, 0);
    context->dsSelector = Selector(USER_PRIVILEGE, false, 1);
    
    context->refCount = 0;

    // swap address spaces so we can put the arg blocks in right place
    // Switch_To_Address_Space(context);
    
    return context;

    fail:
    /* We failed; release any allocated memory */
    Disable_Interrupts();
    if (context != 0) {
        Free(context);
    }
    if (dir != 0) {
        Free_Page(dir);
    }
    Enable_Interrupts();

    return 0;
    
}

bool Validate_User_Memory(struct User_Context * userContext,
                          ulong_t userAddr, ulong_t bufSize) {
    ulong_t avail;

    if (userAddr >= USER_VM_SIZE)
        return false;

    avail = USER_VM_SIZE - userAddr;
    if (bufSize > avail)
        return false;

    return true;
}

/* ----------------------------------------------------------------------
 * Public functions
 * ---------------------------------------------------------------------- */

/*
 * Destroy a User_Context object, including all memory
 * and other resources allocated within it.
 */
void Destroy_User_Context(struct User_Context *context) {
    /*
     * Hints:
     * - Free all pages, page tables, and page directory for
     *   the process (interrupts must be disabled while you do this,
     *   otherwise those pages could be stolen by other processes)
     * - Free semaphores, files, and other resources used
     *   by the process
     */ 
    
    int i, j;
    pde_t * dir = context->pageDir;
    
    Disable_Interrupts();
    
    for(i = NUM_PAGE_TABLE_ENTRIES/2; i < NUM_PAGE_TABLE_ENTRIES;i++) {
        if(dir[i].present) {
            pte_t * table = (dir[i].pageTableBaseAddr) << 12;
            for(j = 0; j < NUM_PAGE_TABLE_ENTRIES ;j++) {
              if(table[j].present) {
                ulong_t page = table[j].pageBaseAddr << 12;
                Free_Page(page);
              } else if(table[j].kernelInfo == KINFO_PAGE_ON_DISK) {
                 Free_Space_On_Paging_File(table[j].pageBaseAddr);
              }
            }
            Free_Page(table);
        }
    }
    
    Free_Page(dir);
    
    Enable_Interrupts();
    //TODO_P(PROJECT_VIRTUAL_MEMORY_A,
    //       "Destroy User_Context data structure after process exits");
}

/*
 * Load a user executable into memory by creating a User_Context
 * data structure.
 * Params:
 * exeFileData - a buffer containing the executable to load
 * exeFileLength - number of bytes in exeFileData
 * exeFormat - parsed ELF segment information describing how to
 *   load the executable's text and data segments, and the
 *   code entry point address
 * command - string containing the complete command to be executed:
 *   this should be used to create the argument block for the
 *   process
 * pUserContext - reference to the pointer where the User_Context
 *   should be stored
 *
 * Returns:
 *   0 if successful, or an error code (< 0) if unsuccessful
 */
int Load_User_Program(char *exeFileData, ulong_t exeFileLength,
                      struct Exe_Format *exeFormat, const char *command,
                      struct User_Context **pUserContext) {
    /*
     * Hints:
     * - This will be similar to the same function in userseg.c
     * - Determine space requirements for code, data, argument block,
     *   and stack
     * - Allocate pages for above, map them into user address
     *   space (allocating page directory and page tables as needed)
     * - Fill in initial stack pointer, argument block address,
     *   and code entry point fields in User_Context
     */
    int i, j;
    ulong_t maxva = 0;
    unsigned numArgs;
    ulong_t argBlockSize;
    ulong_t size, argBlockAddr;
    struct User_Context *userContext = 0;

    /* Find maximum virtual address */
    for (i = 0; i < exeFormat->numSegments; ++i) {
        struct Exe_Segment *segment = &exeFormat->segmentList[i];
        ulong_t topva = segment->startAddress + segment->sizeInMemory;  /* FIXME: range check */
        if (topva > maxva)
            maxva = topva;
    }

    /* Determine size required for argument block */
    Get_Argument_Block_Size(command, &numArgs, &argBlockSize);

    /*
     * Now we can determine the size of the memory block needed
     * to run the process.
     */
    size = Round_Up_To_Page(maxva);
    char * memory = Malloc(size);

    /* Create User_Context */
    userContext = Create_User_Context();
    if (userContext == 0)
        return -1;
    

    /* Load segment data into memory */
    for (i = 0; i < exeFormat->numSegments; ++i) {
        struct Exe_Segment *segment = &exeFormat->segmentList[i];

        memcpy(memory + segment->startAddress,
               exeFileData + segment->offsetInFile, segment->lengthInFile);
    }
    
    // Alloc the first few pages for Code/Data blocks
    int num_pages = size / PAGE_SIZE;
    for (i = 512, j = 1; j < num_pages; j++) {
        if(j == 1024) {
            i++;
            j = 0;
        }
        if(i == 1024) {
            // dammit?
        }
        pte_t * entry = Get_Table_Entry(userContext->pageDir, i, j, true, true);
        void* page = Alloc_Pageable_Page(entry, (i * 1024 + j) * PAGE_SIZE);
        entry->present = 1;
        entry->flags = VM_READ | VM_WRITE | VM_USER;
        entry->pageBaseAddr = (ulong_t)page >> 12;
        memcpy(page, memory + ((i-512) * 1024 + j) * PAGE_SIZE, PAGE_SIZE);
    }
    // Free our junky buffer.
    Free(memory);
    // Arg and stack blocks
    pte_t* entry = Get_Table_Entry(userContext->pageDir, 1023, 1022, true, true);
    ulong_t page = Alloc_Pageable_Page(entry, 0xFFFFE000);
    entry->present = 1;
    entry->flags = VM_READ | VM_WRITE | VM_USER;
    entry->pageBaseAddr = page >> 12;
    
    entry = Get_Table_Entry(userContext->pageDir, 1023, 1023, true, true);
    ulong_t arg_block = Alloc_Pageable_Page(entry, 0xFFFFF000);
    entry->present = 1;
    entry->flags = VM_READ | VM_WRITE | VM_USER;
    entry->pageBaseAddr = arg_block >> 12;
    argBlockAddr = 0x7FFFF000;
    userContext->stack_limit = 0xFFFFE000;
    userContext->size_exe = size;

    /* Format argument block */
    Format_Argument_Block(arg_block, numArgs, argBlockAddr, command);

    /* Fill in code entry point */
    userContext->entryAddr = exeFormat->entryAddr;

    /*
     * Fill in addresses of argument block and stack
     * (They happen to be the same)
     */
    userContext->argBlockAddr = argBlockAddr;
    userContext->stackPointerAddr = argBlockAddr;


    *pUserContext = userContext;
    return 0;
    
    //TODO_P(PROJECT_VIRTUAL_MEMORY_A, "Load user program into address space");
    //return 0;
}

/*
 * Copy data from user buffer into kernel buffer.
 * Returns true if successful, false otherwise.
 */
bool Copy_From_User(void *destInKernel, ulong_t srcInUser, ulong_t numBytes) {
    /*
     * Hints:
     * - Make sure that user page is part of a valid region
     *   of memory
     * - Remember that you need to add 0x80000000 to user addresses
     *   to convert them to kernel addresses, because of how the
     *   user code and data segments are defined
     * - User pages may need to be paged in from disk before being accessed.
     * - Before you touch (read or write) any data in a user
     *   page, **disable the PAGE_PAGEABLE bit**.
     *
     * Be very careful with race conditions in reading a page from disk.
     * Kernel code must always assume that if the struct Page for
     * a page of memory has the PAGE_PAGEABLE bit set,
     * IT CAN BE STOLEN AT ANY TIME.  The only exception is if
     * interrupts are disabled; because no other process can run,
     * the page is guaranteed not to be stolen.
     */
  
     struct User_Context * context = CURRENT_THREAD->userContext;
  
    if(!Validate_User_Memory(context, srcInUser, numBytes)) {
        return false;
    }
  
    memcpy(destInKernel, User_To_Kernel(context, srcInUser), numBytes);
    return true;
  
  //TODO_P(PROJECT_VIRTUAL_MEMORY_A, "Copy user data to kernel buffer");
}

/*
 * Copy data from kernel buffer into user buffer.
 * Returns true if successful, false otherwise.
 */
bool Copy_To_User(ulong_t destInUser, void *srcInKernel, ulong_t numBytes) {
    /*
     * Hints:
     * - Same as for Copy_From_User()
     * - Also, make sure the memory is mapped into the user
     *   address space with write permission enabled
     */
     struct User_Context * context = CURRENT_THREAD->userContext;
  
    if(!Validate_User_Memory(context, destInUser, numBytes)) {
        return false;
    }
  
    memcpy(User_To_Kernel(context, destInUser), srcInKernel, numBytes);
    return true;
  
  //TODO_P(PROJECT_VIRTUAL_MEMORY_A, "Copy kernel data to user buffer");
}

/*
 * Switch to user address space.
 */
void Switch_To_Address_Space(struct User_Context *userContext) {
    /*
     * - If you are still using an LDT to define your user code and data
     *   segments, switch to the process's LDT
     * - 
     */
    ushort_t ldtSelector;

    /* Switch to the LDT of the new user context */
    ldtSelector = userContext->ldtSelector;
    __asm__ __volatile__("lldt %0"::"a"(ldtSelector)
        );
    
    Set_PDBR(userContext->pageDir);
    
    //TODO_P(PROJECT_VIRTUAL_MEMORY_A,
    //       "Switch_To_Address_Space() using paging");
}
