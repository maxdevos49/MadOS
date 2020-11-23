#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

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

int main(int argc, char *argv[])
{
    int num_headers = (argc - 1) / 2;

    //Only supports 64 files
    struct initrd_file_header headers[64];

    printf("size of header: %ld\n", sizeof(struct initrd_file_header));

    //offset
    unsigned int off = (sizeof(struct initrd_file_header) * 64) + sizeof(struct initrd_header);

    int i;
    for (i = 0; i < num_headers; i++)
    {

        printf("writing file %s->%s at 0x%x\n", argv[i * 2 + 1], argv[i * 2 + 2], off);
        strcpy(headers[i].name, argv[i * 2 + 2]);

        headers[i].offset = off;
        FILE *stream = fopen(argv[i * 2 + 1], "r");
        if (stream == 0)
        {
            printf("Error: file not found: %s\n", argv[i * 2 + 1]);
            return 1;
        }

        fseek(stream, 0, SEEK_END);
        headers[i].length = ftell(stream);
        off += headers[i].length;

        fclose(stream);
        headers[i].magic = 0xBF;
    }

    FILE *write_stream = fopen("./initrd.img", "w");
    uint8_t *data = (uint8_t *)malloc(off);

    fwrite(&num_headers, sizeof(int), 1, write_stream);
    fwrite(headers, sizeof(struct initrd_file_header), 64, write_stream);

    for (i = 0; i < num_headers; i++)
    {
        FILE *read_stream = fopen(argv[i * 2 + 1], "r");
        uint8_t *buf = (uint8_t *)malloc(headers[i].length);
        fread(buf, 1, headers[i].length, read_stream);
        fwrite(buf, 1, headers[i].length, write_stream);
        fclose(read_stream);
        free(buf);
    }

    fclose(write_stream);
    free(data);

    return 0;
}