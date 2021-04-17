#ifndef __KERNEL_FILESYSTEMS_INITRD_H
#define __KERNEL_FILESYSTEMS_INITRD_H 1

#include <kernel/filesystems/fs.h>

struct initrd_header
{
    uint32_t nfiles;
};

struct initrd_file_header
{
    uint8_t magic;   //For error checking
    char name[64];   //File name
    uint32_t offset; //Offset in rd where file starts
    uint32_t length; //Length in bytes
};

struct fs_node *INITRD_init();

#endif