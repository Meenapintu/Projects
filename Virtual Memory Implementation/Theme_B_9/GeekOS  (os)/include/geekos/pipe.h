#include <geekos/vfs.h>

extern struct File_Ops Pipe_Read_Ops;
extern struct File_Ops Pipe_Write_Ops;

int Pipe_Create(struct File **read_file, struct File **write_file);
int Pipe_Read(struct File *f, void *buf, ulong_t numBytes);
int Pipe_Write(struct File *f, void *buf, ulong_t numBytes);
int Pipe_Close(struct File *f);
