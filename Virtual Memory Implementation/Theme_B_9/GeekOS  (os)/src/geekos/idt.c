/*
 * GeekOS IDT initialization code
 * Copyright (c) 2001, David H. Hovemeyer <daveho@cs.umd.edu>
 * $Revision: 1.9 $
 * 
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "COPYING".
 */

#include <geekos/kassert.h>
#include <geekos/defs.h>
#include <geekos/idt.h>

/* ----------------------------------------------------------------------
 * Private data and functions
 * ---------------------------------------------------------------------- */

/*
 * TODO: dynamically allocate this?
 */
static union IDT_Descriptor s_IDT[NUM_IDT_ENTRIES];

/*
 * These symbols are defined in lowlevel.asm, and define the
 * size of the interrupt entry point table and the sizes
 * of the individual entry points.  This gives us sufficient
 * information to build the IDT.
 */
extern char g_entryPointTableStart, g_entryPointTableEnd;
extern int g_handlerSizeNoErr, g_handlerSizeErr;

/*
 * Table of C interrupt handler functions.
 * Note that this is public only because it is used
 * in lowlevel.asm.  Other code should not refer to it.
 */
Interrupt_Handler g_interruptTable[NUM_IDT_ENTRIES];


/* ----------------------------------------------------------------------
 * Public functions
 * ---------------------------------------------------------------------- */

/*
 * Initialize the Interrupt Descriptor Table.
 * This will allow us to install C handler functions
 * for interrupts, both processor-generated and
 * those generated by external hardware.
 */
void Init_IDT(void) {
    int i;
    ushort_t limitAndBase[3];
    ulong_t idtBaseAddr = (ulong_t) s_IDT;
    ulong_t tableBaseAddr = (ulong_t) & g_entryPointTableStart;
    ulong_t addr;

    Print("Initializing IDT...\n");

    /* Make sure the layout of the entry point table is as we expect. */
    KASSERT(g_handlerSizeNoErr == g_handlerSizeErr);
    KASSERT((&g_entryPointTableEnd - &g_entryPointTableStart) ==
            g_handlerSizeNoErr * NUM_IDT_ENTRIES);

    /*
     * Build the IDT.
     * We're taking advantage of the fact that all of the
     * entry points are laid out consecutively, and that they
     * are all padded to be the same size.
     */
    for (i = 0, addr = tableBaseAddr; i < NUM_IDT_ENTRIES; ++i) {
        /*
         * All interrupts except for the syscall interrupt
         * must have kernel privilege to access.
         */
        int dpl = (i == SYSCALL_INT) ? USER_PRIVILEGE : KERNEL_PRIVILEGE;
        Init_Interrupt_Gate(&s_IDT[i], addr, dpl);
        addr += g_handlerSizeNoErr;
    }

    /*
     * Cruft together a 16 bit limit and 32 bit base address
     * to load into the IDTR.
     */
    limitAndBase[0] = 8 * NUM_IDT_ENTRIES;
    limitAndBase[1] = idtBaseAddr & 0xffff;
    limitAndBase[2] = idtBaseAddr >> 16;

    /* Install the new table in the IDTR. */
    Load_IDTR(limitAndBase);
}

/*
 * Initialize an interrupt gate with given handler address
 * and descriptor privilege level.
 */
void Init_Interrupt_Gate(union IDT_Descriptor *desc, ulong_t addr, int dpl) {
    KASSERT(desc);              /* NULL when memory is insufficient. */
    desc->ig.offsetLow = addr & 0xffff;
    desc->ig.segmentSelector = KERNEL_CS;
    desc->ig.reserved = 0;
    desc->ig.signature = 0x70;  /* == 01110000b */
    desc->ig.dpl = dpl;
    desc->ig.present = 1;
    desc->ig.offsetHigh = addr >> 16;
}

/*
 * Install a C handler function for given interrupt.
 * This is a lower-level notion than an "IRQ", which specifically
 * means an interrupt triggered by external hardware.
 * This function can install a handler for ANY interrupt.
 */
void Install_Interrupt_Handler(int interrupt, Interrupt_Handler handler) {
    KASSERT(interrupt >= 0 && interrupt < NUM_IDT_ENTRIES);
    g_interruptTable[interrupt] = handler;
}
