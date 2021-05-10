#ifndef __BOOT_STDIO_FILE_H
#define __BOOT_STDIO_FILE_H 1

#include "../stdint.h"

struct __file_stream
{
    //     int level;             /* fill/empty level of buffer */
    uint32_t flags;        /* File status flags          */
    int8_t file_descriptor;               /* File descriptor            */
    // unsigned char hold;    /* Ungetc char if no buffer   */
    uint64_t buffer_size;             /* Buffer size                */
    uint8_t *buffer; /* Data transfer buffer       */
    uint8_t *current_pointer;   /* Current active pointer     */
    // uint32_t is_temp;       /* Temporary file indicator   */
};

#endif