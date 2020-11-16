#ifndef __KERNEL_FILESYSTEM_FS_H
#define __KERNEL_FILESYSTEM_FS_H 0

#include <stdint.h>
#include <stdlib.h>

#define FS_FILE 0x01
#define FS_DIRECTORY 0x02
#define FS_CHARDEVICE 0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE 0x05
#define FS_SYMLINK 0x06
#define FS_MOUNTPOINT 0x08

struct fs_node;
struct dirent;

typedef uint32_t (*read_type_t)(struct fs_node *, uint32_t, uint32_t, uint8_t *);
typedef uint32_t (*write_type_t)(struct fs_node *, uint32_t, uint32_t, uint8_t *);
typedef void (*open_type_t)(struct fs_node *);
typedef void (*close_type_t)(struct fs_node *);

typedef struct dirent *(*readdir_type_t)(struct fs_node *, uint32_t);
typedef struct fs_node *(*finddir_type_t)(struct fs_node *, char *name);

struct fs_node
{
    char name[128]; //Filename
    uint32_t mask;  //The permission Mask
    uint32_t uid;   //The owning use
    uint32_t gid;   //The owning group
    uint32_t flags; //Includes the node type.
    uint32_t inode; // This is a device-specific - provides a way for a filesystem to identify files
    size_t length;  //Size of file in bytes
    uint32_t impl;  //A implemenation defined number

    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;

    readdir_type_t readdir; //Returns the nth child of a directory
    finddir_type_t finddir; //Try to find a child in a directory by name.

    struct fs_node *ptr; //Used by mountpoints and symlinks
};

struct dirent{
    char name[128]; // Filename
    uint32_t ino;   // Inode number. Required by POSIX
};

extern struct fs_node *fs_node; // Filesystem root

uint32_t read_fs(struct fs_node *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_fs(struct fs_node *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_fs(struct fs_node *node, uint8_t read, uint8_t write);
void close_fs(struct fs_node *node);

struct dirent *readdir_fs(struct fs_node *node, uint32_t index);
struct fs_node *finddir_fs(struct fs_node *node, char *name);

#endif
