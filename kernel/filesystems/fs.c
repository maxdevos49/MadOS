#include <kernel/filesystems/fs.h>

//Global
struct fs_node *fs_root = NULL;

uint32_t read_fs(struct fs_node *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    if (node->read != NULL)
        return node->read(node, offset, size, buffer);
    else
        return NULL;
}

uint32_t write_fs(struct fs_node *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    if (node->write != NULL)
        return node->write(node, offset, size, buffer);
    else
        return NULL;
}

void open_fs(struct fs_node *node) //TODO Careful
{
    if (node->open != NULL)
        node->open(node);
}

void close_fs(struct fs_node *node)
{
    if (node->close != NULL)
        node->close(node);
}

struct dirent *readdir_fs(struct fs_node *node, uint32_t index)
{
    if ((node->flags & 0x7) == FS_DIRECTORY && node->readdir != NULL)
        return node->readdir(node, index);
    else
        return NULL;
}

struct fs_node *finddir_fs(struct fs_node *node, char *name)
{
    if ((node->flags & 0x7) == FS_DIRECTORY && node->finddir != NULL)
        return node->finddir(node, name);
    else
        return NULL;

}
