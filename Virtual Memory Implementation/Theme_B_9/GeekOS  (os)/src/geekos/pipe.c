#include <geekos/pipe.h>
#include <geekos/malloc.h>
#include <geekos/string.h>
#include <geekos/errno.h>
#include <geekos/projects.h>


struct File_Ops Pipe_Read_Ops =
    { NULL, Pipe_Read, NULL, NULL, Pipe_Close, NULL };
struct File_Ops Pipe_Write_Ops =
    { NULL, NULL, Pipe_Write, NULL, Pipe_Close, NULL };

int Pipe_Create(struct File **read_file, struct File **write_file) {
    TODO_P(PROJECT_PIPE, "Create a pipe");
}

int Pipe_Read(struct File *f, void *buf, ulong_t numBytes) {
    TODO_P(PROJECT_PIPE, "Pipe read");
}

int Pipe_Write(struct File *f, void *buf, ulong_t numBytes) {
    TODO_P(PROJECT_PIPE, "Pipe write");
}

int Pipe_Close(struct File *f) {
    TODO_P(PROJECT_PIPE, "Pipe close");
    return 0;
}
