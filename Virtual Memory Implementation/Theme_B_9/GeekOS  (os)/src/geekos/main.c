/*
 * GeekOS C code entry point
 * Copyright (c) 2001,2003,2004 David H. Hovemeyer <daveho@cs.umd.edu>
 * Copyright (c) 2003, Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 * Copyright (c) 2004, Iulian Neamtiu <neamtiu@cs.umd.edu>
 * $Revision: 1.53 $
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "COPYING".
 */

#include <geekos/bootinfo.h>
#include <geekos/string.h>
#include <geekos/screen.h>
#include <geekos/mem.h>
#include <geekos/crc32.h>
#include <geekos/tss.h>
#include <geekos/int.h>
#include <geekos/kthread.h>
#include <geekos/trap.h>
#include <geekos/timer.h>
#include <geekos/keyboard.h>
#include <geekos/dma.h>
#include <geekos/ide.h>
#include <geekos/floppy.h>
#include <geekos/pfat.h>
#include <geekos/vfs.h>
#include <geekos/user.h>
#include <geekos/paging.h>
#include <geekos/gosfs.h>
#include <geekos/gfs2.h>
#include <geekos/net/ne2000.h>
#include <geekos/net/net.h>
#include <geekos/net/netbuf.h>
#include <geekos/net/arp.h>
#include <geekos/alarm.h>
#include <geekos/net/ip.h>
#include <geekos/net/routing.h>
#include <geekos/net/socket.h>
#include <geekos/net/rip.h>
#include <geekos/projects.h>
#include <geekos/sound.h>


/*
 * Define this for a self-contained boot floppy
 * with a PFAT filesystem.  (Target "fd_aug.img" in
 * the makefile.)
 */
/*#define FD_BOOT*/

#ifdef FD_BOOT
#  define ROOT_DEVICE "fd0"
#  define ROOT_PREFIX "a"
#else
#  define ROOT_DEVICE "ide0"
#  define ROOT_PREFIX "c"
#endif

#define INIT_PROGRAM "/" ROOT_PREFIX "/shell.exe"



static void Mount_Root_Filesystem(void);
static void Spawn_Init_Process(void);

/*
 * Kernel C code entry point.
 * Initializes kernel subsystems, mounts filesystems,
 * and spawns init process.
 */

extern void checkPaging(void);

void Main(struct Boot_Info *bootInfo) {
    Init_BSS();
    Init_Screen();
    Init_Mem(bootInfo);
    Init_CRC32();
    Init_TSS();
    Init_Interrupts();
    //TODO_P(PROJECT_VIRTUAL_MEMORY_A,
    //       "initialize virtual memory page tables.");
    Init_VM(bootInfo);
    Init_Scheduler();
    Init_Traps();
    Init_Timer();
    Init_Keyboard();
    Init_DMA();
    /* Init_Floppy(); *//* floppy initialization hangs on virtualbox */
    Init_IDE();
    Init_PFAT();
    Init_GFS2();
    Init_GOSFS();
    Init_Alarm();

    /* Initialize Networking */
    Init_Network_Devices();
    Init_ARP_Protocol();
    Init_IP();
    Init_Routing();
    Init_Sockets();
    Init_RIP();
    /* End networking subsystem init */

    /* Initialize Sound */
    Init_Sound_Devices();
    /* End sound init */

    Mount_Root_Filesystem();

    // TODO_P(PROJECT_VIRTUAL_MEMORY_A, "initialize page file.");
    Init_Paging();

    Set_Current_Attr(ATTRIB(BLACK, GREEN | BRIGHT));
    Print("Welcome to GeekOS!\n");
    Set_Current_Attr(ATTRIB(BLACK, GRAY));

    TODO_P(PROJECT_SOUND, "play startup sound");


    Spawn_Init_Process();

    /* Now this thread is done. */
    Exit(0);
}



static void Mount_Root_Filesystem(void) {
    if (Mount(ROOT_DEVICE, ROOT_PREFIX, "pfat") != 0)
        Print("Failed to mount /" ROOT_PREFIX " filesystem\n");
    else
        Print("Mounted /" ROOT_PREFIX " filesystem!\n");

}






static void Spawn_Init_Process(void) {
    int rc;
    struct Kernel_Thread *initProcess;

    /* Load and run a.exe, the "init" process */
    Print("Spawning init process (%s)\n", INIT_PROGRAM);
    rc = Spawn_Foreground(INIT_PROGRAM, INIT_PROGRAM, &initProcess);

    if (rc != 0) {
        Print("Failed to spawn init process: error code = %d\n", rc);
    } else {
        /* Wait for it to exit */
        int exitCode = Join(initProcess);
        Print("Init process exited with code %d\n", exitCode);
    }
}
