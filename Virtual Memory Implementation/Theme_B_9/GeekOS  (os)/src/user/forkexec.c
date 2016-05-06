#include <conio.h>
#include <process.h>
#include <sched.h>
#include <sema.h>
#include <string.h>
#include <fileio.h>
#include <geekos/errno.h>

int main(int argc, char **argv) {
    int child_pid = Fork();
    int rc;

    if (child_pid > 0) {
        Print("waiting for  %d\n", child_pid);
        rc = Wait(child_pid);
        Print("The child exited %d\n", rc);
    } else if (child_pid == 0) {
        rc = Execl("/c/b.exe", "b program argument");
        Print("exec failed: %d\n", rc);
    } else {
        Print("fork failed: %s (%d)\n", Get_Error_String(child_pid),
              child_pid);
    }
    return 0;
}
