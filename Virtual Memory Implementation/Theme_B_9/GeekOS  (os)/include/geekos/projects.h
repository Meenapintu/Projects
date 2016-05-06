/* Don't print an informative message when running an
   unimplemented function from a project with an incomplete
   TODO */
#define PROJECT_VERBOSITY false
/* traditional project 0 */
#define PROJECT_LIMIT_SYSCALLS false
#define PROJECT_LIMIT_ACTIVE_PROCESSES false
/* traditional project 1 */
#define PROJECT_BACKGROUND_JOBS false
/* traditional project 2 */
#define PROJECT_SIGNALS false
/* traditional project 3 */
#define PROJECT_SEMAPHORES false
#define PROJECT_SCHEDULING false
/* traditional project 4 */
#define PROJECT_VIRTUAL_MEMORY_A false
#define PROJECT_VIRTUAL_MEMORY_B false
/* any file system project */
#define PROJECT_FS false
/* neil's version of project 5 */
#define PROJECT_GFS2 false
/* sound driver */
#define PROJECT_SOUND false

#define PROJECT_GOSFS false
#define PROJECT_USER false


/* networking projects */
#define PROJECT_ETHERNET false
#define PROJECT_TCP false
#define PROJECT_RIP false
#define PROJECT_SOCKETS false
#define PROJECT_RAW_ETHERNET false
#define PROJECT_IP false
#define PROJECT_UDP false
#define PROJECT_TCP false
#define PROJECT_ROUTING false
#define PROJECT_ARP false

/* revised intro projects */
#define PROJECT_PIPE true
#define PROJECT_FORK false

#ifndef TODO
#ifdef GEEKOS
#warning "projects.h file requires geekos/screen.h to be included first in kernel source files"
#else
#include <conio.h>
#define TODO(x) Print(x)
#endif
#endif

#define TODO_P(proj, message)  do  { \
    if(proj) { TODO(__FILE__ ": " message); }  \
    else if(PROJECT_VERBOSITY) { Print("Invoked function of project: " #proj ": " message "\n"); } \
   } while(0)
