#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>

int main(int argc __attribute__ ((unused)), char **argv
         __attribute__ ((unused))) {
    int scr_sem, holdp3_sem;
    scr_sem = Open_Semaphore("screen", 1);      /* register for screen use */
    holdp3_sem = Open_Semaphore("holdp3_sem", 0);

    P(holdp3_sem);

    P(scr_sem);
    Print("p3 executed\n");
    V(scr_sem);

    V(holdp3_sem);

    Close_Semaphore(scr_sem);
    Close_Semaphore(holdp3_sem);

    return 0;
}
