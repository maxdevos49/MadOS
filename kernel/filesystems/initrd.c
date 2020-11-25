#include <kernel/filesystems/initrd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern struct initrd_header initrd_data_header;

static struct initrd_header *initrd_header;     //File System header
static struct initrd_file_header *file_headers; //List of file headers

static struct fs_node *initrd_root; //Root directory node
static struct fs_node *initrd_dev;  //Dev directory node

static struct fs_node *root_nodes;
//List of file nodes
static uint32_t root_node_count; //Number of file nodes
static struct dirent dirent;

/**
 * Reads from a fs_node in a ramdisk filesystem
 * */
static uint32_t initrd_read(struct fs_node *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{

    struct initrd_file_header header = file_headers[node->inode];

    if (offset > header.length)
        return NULL;

    if (offset + size > header.length)
        size = header.length - offset;

    memcpy(buffer, (void *)(long)(header.offset + offset), size);

    return size;
}

static struct dirent *initrd_readdir(struct fs_node *node, uint32_t index)
{
    if (node == initrd_root && index == 0)
    {
        strcpy(dirent.name, "dev");
        dirent.name[3] = 0; // Make sure the string is NULL-terminated.
        dirent.ino = 0;

        return &dirent;
    }

    if (index - 1 >= root_node_count)
    {
        return NULL;
    }

    strcpy(dirent.name, root_nodes[index - 1].name);

    dirent.name[strlen(root_nodes[index - 1].name)] = 0; // Make sure the string is NULL-terminated.
    dirent.ino = root_nodes[index - 1].inode;

    return &dirent;
}

static struct fs_node *initrd_finddir(struct fs_node *node, char *name)
{

    if (node == initrd_root && !strcmp(name, "dev"))
        return initrd_dev;

    uint32_t i;
    for (i = 0; i < root_node_count; i++)
        if (!strcmp(name, root_nodes[i].name))
            return &root_nodes[i];

    return NULL;
}

struct fs_node *INITRD_init()
{
    printf("Initlizing Ram Disk\n");

    initrd_header = (struct initrd_header *)&initrd_data_header;
    file_headers = (struct initrd_file_headers *)((long long)&initrd_data_header + (long long)sizeof(struct initrd_header));

    //root directory
    initrd_root = (struct fs_node *)malloc(sizeof(struct fs_node));
    strcpy(initrd_root->name, "initrd");
    initrd_root->mask = initrd_root->uid = initrd_root->gid = initrd_root->inode = initrd_root->length = NULL;
    initrd_root->flags = FS_DIRECTORY;
    initrd_root->read = NULL;
    initrd_root->write = NULL;
    initrd_root->open = NULL;
    initrd_root->close = NULL;
    initrd_root->readdir = &initrd_readdir;
    initrd_root->finddir = &initrd_finddir;
    initrd_root->ptr = NULL;
    initrd_root->impl = NULL;

    initrd_dev = (struct fs_node *)malloc(sizeof(struct fs_node));

    strcpy(initrd_dev->name, "dev");
    initrd_dev->mask = initrd_dev->uid = initrd_dev->gid = initrd_dev->inode = initrd_dev->length = NULL;
    initrd_dev->flags = FS_DIRECTORY;
    initrd_dev->read = NULL;
    initrd_dev->write = NULL;
    initrd_dev->open = NULL;
    initrd_dev->close = NULL;
    initrd_dev->readdir = &initrd_readdir;
    initrd_dev->finddir = &initrd_finddir;
    initrd_dev->ptr = NULL;
    initrd_dev->impl = NULL;

    root_nodes = (struct fs_node *)malloc(sizeof(struct fs_node) * initrd_header->nfiles);
    root_node_count = initrd_header->nfiles;

    uint32_t i;
    for (i = 0; i < root_node_count; i++)
    {
        // Edit the file's header - currently it holds the file offset
        // relative to the start of the ramdisk. We want it relative to the start
        // of memory.
        file_headers[i].offset += (uint32_t)&initrd_data_header;

        // Create a new file node.
        strcpy(root_nodes[i].name, (char *)&file_headers[i].name);

        root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
        root_nodes[i].length = file_headers[i].length;
        root_nodes[i].inode = i;
        root_nodes[i].flags = FS_FILE;
        root_nodes[i].read = &initrd_read;
        root_nodes[i].write = 0;
        root_nodes[i].readdir = 0;
        root_nodes[i].finddir = 0;
        root_nodes[i].open = 0;
        root_nodes[i].close = 0;
        root_nodes[i].impl = 0;
    }

    return initrd_root;
}
