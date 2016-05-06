/*
 * GeekOS file system
 * Copyright (c) 2008, David H. Hovemeyer <daveho@cs.umd.edu>, 
 * Neil Spring <nspring@cs.umd.edu>, Aaron Schulman <schulman@cs.umd.edu>
 * $Revision: 1.56 $
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "COPYING".
 */

#include <limits.h>
#include <geekos/errno.h>
#include <geekos/kassert.h>
#include <geekos/screen.h>
#include <geekos/malloc.h>
#include <geekos/string.h>
#include <geekos/bitset.h>
#include <geekos/synch.h>
#include <geekos/bufcache.h>
#include <geekos/gfs2.h>
#include <geekos/projects.h>

/* ----------------------------------------------------------------------
 * Private data and functions
 * ---------------------------------------------------------------------- */


/* ----------------------------------------------------------------------
 * Implementation of VFS operations
 * ---------------------------------------------------------------------- */

/*
 * Get metadata for given file.
 */
static int GFS2_FStat(struct File *file, struct VFS_File_Stat *stat) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem FStat operation");
    return 0;
}

/*
 * Read data from current position in file.
 */
static int GFS2_Read(struct File *file, void *buf, ulong_t numBytes) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem read operation");

    return EUNSUPPORTED;
}

/*
 * Write data to current position in file.
 */
static int GFS2_Write(struct File *file, void *buf, ulong_t numBytes) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem write operation");
    return EUNSUPPORTED;
}


/*
 * Seek to a position in file.
 */
static int GFS2_Seek(struct File *file, ulong_t pos) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem seek operation");
    return EUNSUPPORTED;
}

/*
 * Close a file.
 */
static int GFS2_Close(struct File *file) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem close operation");
    return EUNSUPPORTED;
}

/*static*/ struct File_Ops s_gfs2FileOps = {
    &GFS2_FStat,
    &GFS2_Read,
    &GFS2_Write,
    &GFS2_Seek,
    &GFS2_Close,
    0,                          /* Read_Entry */
};

/*
 * Stat operation for an already open directory.
 */
static int GFS2_FStat_Directory(struct File *dir, struct VFS_File_Stat *stat) {
    /* may be unused. */
    TODO_P(PROJECT_GFS2, "GeekOS filesystem FStat directory operation");
    return 0;
}

/*
 * Directory Close operation.
 */
static int GFS2_Close_Directory(struct File *dir) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem Close directory operation");
    return EUNSUPPORTED;
}

/*
 * Read a directory entry from an open directory.
 */
static int GFS2_Read_Entry(struct File *dir, struct VFS_Dir_Entry *entry) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem Read_Entry operation");
    return EUNSUPPORTED;
}

/*static*/ struct File_Ops s_gfs2DirOps = {
    &GFS2_FStat,
    0,                          /* Read */
    0,                          /* Write */
    0,                          /* Seek */
    &GFS2_Close_Directory,
    &GFS2_Read_Entry,
};



/*
 * Open a file named by given path.
 */
static int GFS2_Open(struct Mount_Point *mountPoint, const char *path,
                     int mode, struct File **pFile) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem open operation");
    return EUNSUPPORTED;
}

/*
 * Create a directory named by given path.
 */
static int GFS2_Create_Directory(struct Mount_Point *mountPoint,
                                 const char *path) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem create directory operation");
    return EUNSUPPORTED;
}

/*
 * Open a directory named by given path.
 */
static int GFS2_Open_Directory(struct Mount_Point *mountPoint,
                               const char *path, struct File **pDir) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem open directory operation");
    return EUNSUPPORTED;
}

/*
 * Open a directory named by given path.
 */
static int GFS2_Delete(struct Mount_Point *mountPoint, const char *path) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem delete operation");
    return EUNSUPPORTED;
}

/*
 * Get metadata (size, permissions, etc.) of file named by given path.
 */
static int GFS2_Stat(struct Mount_Point *mountPoint, const char *path,
                     struct VFS_File_Stat *stat) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem stat operation");
    return EUNSUPPORTED;
}

/*
 * Synchronize the filesystem data with the disk
 * (i.e., flush out all buffered filesystem data).
 */
static int GFS2_Sync(struct Mount_Point *mountPoint) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem sync operation");
    return EUNSUPPORTED;
}

/*static*/ struct Mount_Point_Ops s_gfs2MountPointOps = {
    &GFS2_Open,
    &GFS2_Create_Directory,
    &GFS2_Open_Directory,
    &GFS2_Stat,
    &GFS2_Sync,
    &GFS2_Delete,
    0,                          /* setuid  */
    0,                          /* acl  */
};

static int GFS2_Format(struct Block_Device *blockDev __attribute__ ((unused))) {
    TODO_P(PROJECT_GFS2,
           "DO NOT IMPLEMENT: There is no format operation for GFS2");
    return EUNSUPPORTED;
}

static int GFS2_Mount(struct Mount_Point *mountPoint) {
    TODO_P(PROJECT_GFS2, "GeekOS filesystem mount operation");
    return EUNSUPPORTED;
}

static struct Filesystem_Ops s_gfs2FilesystemOps = {
    &GFS2_Format,
    &GFS2_Mount,
};

/* ----------------------------------------------------------------------
 * Public functions
 * ---------------------------------------------------------------------- */

void Init_GFS2(void) {
    Register_Filesystem("gfs2", &s_gfs2FilesystemOps);
}
