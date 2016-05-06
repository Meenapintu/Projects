/*
 * Paging (virtual memory) support
 * Copyright (c) 2003, Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 * Copyright (c) 2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * $Revision: 1.56 $
 * 
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "COPYING".
 */

#include <geekos/string.h>
#include <geekos/int.h>
#include <geekos/idt.h>
#include <geekos/kthread.h>
#include <geekos/kassert.h>
#include <geekos/bitset.h>
#include <geekos/screen.h>
#include <geekos/mem.h>
#include <geekos/malloc.h>
#include <geekos/gdt.h>
#include <geekos/segment.h>
#include <geekos/user.h>
#include <geekos/vfs.h>
#include <geekos/crc32.h>
#include <geekos/paging.h>
#include <geekos/errno.h>
#include <geekos/projects.h>

#include <geekos/timer.h>

/* ----------------------------------------------------------------------
 * Public data
 * ---------------------------------------------------------------------- */

pde_t* kernelDir;
void * bitSet;
uint_t numberOfPagesOnDisk;

/* ----------------------------------------------------------------------
 * Private functions/data
 * ---------------------------------------------------------------------- */

#define SECTORS_PER_PAGE (PAGE_SIZE / SECTOR_SIZE)

/*
 * flag to indicate if debugging paging code
 */
int debugFaults = 0;
#define Debug(args...) if (debugFaults) Print(args)

struct Paging_Device * pageFile;

/*
 * Print diagnostic information for a page fault.
 */
static void Print_Fault_Info(uint_t address, faultcode_t faultCode) {
    extern uint_t g_freePageCount;

    Print("Pid %d: ", CURRENT_THREAD->pid);
    Print("\n Page Fault received, at address %p (%d pages free)\n",
          (void *)address, g_freePageCount);
    if (faultCode.protectionViolation)
        Print("   Protection Violation, ");
    else
        Print("   Non-present page, ");
    if (faultCode.writeFault)
        Print("Write Fault, ");
    else
        Print("Read Fault, ");
    if (faultCode.userModeFault)
        Print("in User Mode\n");
    else
        Print("in Supervisor Mode\n");
}


/*
 Clock for page time. Copied from mem.c's evicting.
 */
void clock() {
    struct Page * g_pageList = get_page_list();
    unsigned int i;
    struct Page *update;
    for (i = 0; i < s_numPages; i++) {
        if ((g_pageList[i].flags & PAGE_PAGEABLE) &&
            (g_pageList[i].flags & PAGE_ALLOCATED)) {
            update = &g_pageList[i];
            if(update->entry->present && update->entry->accesed) {
                update->clock = g_numTicks;
                update->entry->accesed = 0;
            }
        }
    }
}


void Page_In (pte_t * entry, ulong_t address) {
    Print("VM manager Doing Page_In for address : %lu  .\n",address);
    ulong_t page_addr = Alloc_Pageable_Page(entry,
            Round_Down_To_Page(address));
    struct Page * page = Get_Page(page_addr);
    /* Make the page temporarily unpageable 
     * (can't let another process steal it) */
    page->flags &= ~(PAGE_PAGEABLE);
    /* Lock the page so it cannot be freed while we're writing */
    page->flags |= PAGE_LOCKED;
    Enable_Interrupts();
    Read_From_Paging_File(page_addr, Round_Down_To_Page(address),
            entry->pageBaseAddr);
    Disable_Interrupts();
    Free_Space_On_Paging_File(entry->pageBaseAddr);
    entry->pageBaseAddr = page_addr >> 12;
    entry->present = 1;
    entry->kernelInfo = 0;
    entry->accesed = 1;
    page->clock = g_numTicks;
    // unlock page and make it pageable again
    page->flags |= PAGE_PAGEABLE;
    page->flags &= ~(PAGE_LOCKED);
    Print("Page Loaded in memory ..\n");
}

/*
 * Handler for page faults.
 * You should call the Install_Interrupt_Handler() function to
 * register this function as the handler for interrupt 14.
 */
/*static*/ void Page_Fault_Handler(struct Interrupt_State *state) {
    Print("PageFault Occured  !!!!!!!!!!!!! \n");
    ulong_t address;
    faultcode_t faultCode;
    struct User_Context * context = CURRENT_THREAD->userContext;

    KASSERT(!Interrupts_Enabled());

    /* Get the address that caused the page fault */
    address = Get_Page_Fault_Address();
    Debug("Page fault @%lx\n", address);

    /* Get the fault code */
    faultCode = *((faultcode_t *) & (state->errorCode));
    
    Print_Fault_Info(address, faultCode);
  
    /* rest of your handling code here */
    // TODO_P(PROJECT_VIRTUAL_MEMORY_B, "handle page faults");
    pte_t * entry;
    
    pde_t * dir_entry = &(kernelDir[PAGE_DIRECTORY_INDEX(address)]);
    if(dir_entry->present == 0 && dir_entry->kernelInfo == KINFO_PAGE_ON_DISK) {
        // table on disk
        Page_In((pte_t *) dir_entry, 0);
        memcpy(context->pageDir, kernelDir, 512 * sizeof(pde_t));
    }
    
    if(context) {
        entry = Get_Table_Entry(context->pageDir,
                PAGE_DIRECTORY_INDEX(address), PAGE_TABLE_INDEX(address),
                false, false);
        ulong_t * stack_limit = &(context->stack_limit);
        if(entry && entry->kernelInfo == KINFO_PAGE_ON_DISK) {
            Page_In(entry, address);
            goto done;
        } else if (address > USER_VM_START + context->size_exe
                && (*stack_limit - address) < PAGE_SIZE) {
            entry = Get_Table_Entry(context->pageDir,
                PAGE_DIRECTORY_INDEX(address), PAGE_TABLE_INDEX(address),
                true, true);
            *stack_limit -= PAGE_SIZE;
            Print("Loading The Page into addr : %p .\n",(void *)*stack_limit);
            ulong_t page = Alloc_Pageable_Page(entry, *stack_limit);
            entry->present = 1;
            entry->flags = VM_READ | VM_WRITE | VM_USER;
            entry->pageBaseAddr = page >> 12;
            Print("Page Loaded .\n");
            goto done;
        }
        else if(address > USER_VM_END)
        {
            goto error;
        }
        else
        {
            entry = Get_Table_Entry(context->pageDir,
                PAGE_DIRECTORY_INDEX(address), PAGE_TABLE_INDEX(address),
                true, true);
            *stack_limit -= PAGE_SIZE;
            Print("Loading The Page into addr : %p .\n",(void *)*stack_limit);
            ulong_t page = Alloc_Pageable_Page(entry, *stack_limit);
            entry->present = 1;
            entry->flags = VM_READ | VM_WRITE | VM_USER;
            entry->pageBaseAddr = page >> 12;
            Print("Page Loaded .\n");
            goto done;
        } 
    }
    error:
    Print("Unexpected Page Fault received\n");
    //Print_Fault_Info(address, faultCode);
    Dump_Interrupt_State(state);
    /* user faults just kill the process */
    if (!faultCode.userModeFault)
        KASSERT0(0, "unhandled kernel-mode page fault.");

    /* For now, just kill the thread/process. */
    //Exit(-1);
    done:
    clock();
    Print("****************\n");

    return;
}

/* ----------------------------------------------------------------------
 * Public functions
 * ---------------------------------------------------------------------- */


/*
 * Initialize virtual memory by building page tables
 * for the kernel and physical memory.
 */
void Init_VM(struct Boot_Info *bootInfo) {
        /*
     * Hints:
     * - Build kernel page directory and page tables
     * - Call Enable_Paging() with the kernel page directory
     * - Install an interrupt handler for interrupt 14,
     *   page fault
     * - Do not map a page at address 0; this will help trap
     *   null pointer references
     */
//  TODO_P(PROJECT_VIRTUAL_MEMORY_A,
//          "Build initial kernel page directory and page tables");
    uint_t numberOfPages = bootInfo->memSizeKB >> (PAGE_POWER-10);
    Print("Initializing Paging.\n");
    pde_t *initDir = Alloc_Page();
    memset(initDir, 0, PAGE_SIZE);
    
    ulong_t i = 0;
    while(numberOfPages>0) {
        pte_t *table = Alloc_Pageable_Page(((pte_t*)(&(initDir[i]))), 0);
        memset(table, 0, PAGE_SIZE);
        
        initDir[i].present = 1;
        initDir[i].pageTableBaseAddr = (ulong_t)table >> PAGE_POWER;
        initDir[i].flags = VM_READ | VM_WRITE;
        
        ulong_t j = 0;
        while(j<1024 && numberOfPages>0) {
            if(i!=0 || j!=0) {
                table[j].present = 1;
                table[j].pageBaseAddr = i*1024 + j;
                table[j].flags = VM_READ | VM_WRITE;
            }
            --numberOfPages;
            ++j;
        }
        ++i;
    }
    
    checkPaging();
    Enable_Paging(initDir);
    checkPaging();
    
    Install_Interrupt_Handler(14, &Page_Fault_Handler);
    Print("Installed Interrupt Handler.\n");
    kernelDir = initDir;
    return;
}

/**
 * Initialize paging file data structures.
 * All filesystems should be mounted before this function
 * is called, to ensure that the paging file is available.
 * 
 * Using Bitsets as suggested by Justin
 */
void Init_Paging(void) {
    //  TODO_P(PROJECT_VIRTUAL_MEMORY_B,
//          "Initialize paging file data structures");
    Print("Initilizing Pages,\n");
    pageFile = Get_Paging_Device();
    if(pageFile==0) {
        Print("No page file has been registered\n");
        Exit(-1);
    }
    else {
        numberOfPagesOnDisk = pageFile->numSectors / SECTORS_PER_PAGE;
        bitSet = Create_Bit_Set(numberOfPagesOnDisk);
        Print("Created BitSet Map.\n");
    }
    return;
}
/**
 * Find a free bit of disk on the paging file for this page.
 * Interrupts must be disabled.
 * @return index of free page sized chunk of disk space in
 *   the paging file, or -1 if the paging file is full
 */
int Find_Space_On_Paging_File(void) {
    KASSERT(!Interrupts_Enabled());
    int index = Find_First_Free_Bit(bitSet, numberOfPagesOnDisk);
    if(index >= 0) {
        Set_Bit(bitSet, index);
	index *= SECTORS_PER_PAGE;
        index += pageFile->startSector;
    }
    return index;
    //TODO_P(PROJECT_VIRTUAL_MEMORY_B, "Find free page in paging file");
    //return EUNSUPPORTED;
}

/**
 * Free a page-sized chunk of disk space in the paging file.
 * Interrupts must be disabled.
 * @param pagefileIndex index of the chunk of disk space
 */
void Free_Space_On_Paging_File(int pagefileIndex) {
    KASSERT(!Interrupts_Enabled());
    Clear_Bit(bitSet, (pagefileIndex-pageFile->startSector)/SECTORS_PER_PAGE);
    // TODO_P(PROJECT_VIRTUAL_MEMORY_B, "Free page in paging file");
}

/**
 * Write the contents of given page to the indicated block
 * of space in the paging file.
 * @param paddr a pointer to the physical memory of the page
 * @param vaddr virtual address where page is mapped in user memory
 * @param pagefileIndex the index of the page sized chunk of space
 *   in the paging file
 */
void Write_To_Paging_File(void *paddr, ulong_t vaddr, int pagefileIndex) {
    struct Page *page = Get_Page((ulong_t) paddr);
    KASSERT(!(page->flags & PAGE_PAGEABLE));    /* Page must be locked! */
    int i = 0;
    for (;i < SECTORS_PER_PAGE; i++) {
        Debug("Writing to page file: index: %d addr: %lx vaddr: %8lx\n",
		pagefileIndex + i,
                paddr + i * PAGE_SIZE / SECTORS_PER_PAGE,
		vaddr + i * PAGE_SIZE / SECTORS_PER_PAGE);
        if(Block_Write(pageFile->dev, pagefileIndex + i, 
                paddr + i * PAGE_SIZE / SECTORS_PER_PAGE)) {
            Print("Error on write.\n");
        }
    }
    // TODO_P(PROJECT_VIRTUAL_MEMORY_B, "Write page data to paging file");
}

/**
 * Read the contents of the indicated block
 * of space in the paging file into the given page.
 * @param paddr a pointer to the physical memory of the page
 * @param vaddr virtual address where page will be re-mapped in
 *   user memory
 * @param pagefileIndex the index of the page sized chunk of space
 *   in the paging file
 */
void Read_From_Paging_File(void *paddr, ulong_t vaddr, int pagefileIndex) {
    struct Page *page = Get_Page((ulong_t) paddr);
    KASSERT(!(page->flags & PAGE_PAGEABLE));    /* Page must be locked! */
    int i = 0;
    for (;i < SECTORS_PER_PAGE; i++) {
        Debug("Reading from page file: index: %d addr: %lx vaddr: %8lx\n",
		pagefileIndex + i,
                paddr + i * PAGE_SIZE / SECTORS_PER_PAGE,
		vaddr + i * PAGE_SIZE / SECTORS_PER_PAGE);
        if(Block_Read(pageFile->dev, pagefileIndex + i, 
                paddr + i * PAGE_SIZE / SECTORS_PER_PAGE)) {
            Print("Error on read.\n");
        }
    }
    // TODO_P(PROJECT_VIRTUAL_MEMORY_B, "Read page data from paging file");
}
