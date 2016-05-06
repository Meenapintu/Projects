#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>

int main(int argc __attribute__ ((unused)), char **argv
         __attribute__ ((unused))) {
    int semkey, result;

    /* Unauthorized call */
    result = P(0);
    if (result < 0)
        Print("+ Identified unauthorized call\n");
    else
        Print("- Not checking for authority\n");

    /* Invalid SID */
    result = P(-1);
    if (result < 0)
        Print("+ Identified invalid SID\n");
    else
        Print("- Not checking for invalid SID\n");

    Print("Open_Semaphore() called\n");
    semkey = Open_Semaphore("test", 1);
    Print("Open_Semaphore() returned %d\n", semkey);

    if (semkey < 0)
        return 0;

    Print("P() called\n");
    result = P(semkey);
    Print("P() returned %d\n", result);

    Print("V() called\n");
    result = V(semkey);
    Print("V() returned %d\n", result);


    Print("Close_Semaphore() called\n");
    result = Close_Semaphore(semkey);
    Print("Close_Semaphore() returned %d\n", result);

    /* Unauthorized call */
    result = V(semkey);
    if (result < 0)
        Print("+ Removed authority after finish\n");
    else
        Print("- Not removed authority after finish\n");


    return 0;
}
