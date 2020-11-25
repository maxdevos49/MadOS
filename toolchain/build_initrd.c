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
    uint32_t num_headers = argc - 1;
    printf("\nNumber of Files: %d\n", num_headers);

    struct initrd_file_header *headers = malloc(sizeof(struct initrd_file_header) * num_headers);

    uint32_t data_offset = sizeof(struct initrd_header) + (sizeof(struct initrd_file_header) * num_headers);
    printf("Data offset: %x\n", data_offset);

    printf("Building File Headers:\n");
    for (int i = 0; i < num_headers; i++)
    {
        printf("\t%s\n", argv[i+1]);

        headers[i].magic = 0xbf;
        strcpy(headers[i].name, argv[i + 1]);
        headers[i].offset = data_offset;

        FILE *stream = fopen(argv[i +1], "r");
        if (stream == NULL)
        {
            printf("Error: file not found: %s\n", argv[i +1]);
            return 1;
        }

        fseek(stream, 0, SEEK_END);

        headers[i].length = ftell(stream);

        fclose(stream);

        data_offset += headers[i].length;
    }

    FILE *write_stream = fopen("./initrd.img", "w");

    fwrite(&num_headers, sizeof(struct initrd_header), 1, write_stream);
    fwrite(headers, sizeof(struct initrd_file_header), num_headers, write_stream);

    printf("Writing File Data:\n");
    for (int i = 0; i < num_headers; i++)
    {
        FILE *read_stream = fopen(argv[i + 1], "r");
        if (read_stream == NULL)
        {
            printf("Error: file not found: %s\n", argv[i +1]);
            return 1;
        }

        uint8_t *buf = (uint8_t *)malloc(headers[i].length);
        fread(buf, 1, headers[i].length, read_stream);

        printf("\t%s: %d bytes\n", argv[i+1], headers[i].length);
        fwrite(buf, 1, headers[i].length, write_stream);
        fclose(read_stream);
        free(buf);
    }

    printf("Writing Complete. Total Bytes: %ld\n", ftell(write_stream));
    fclose(write_stream);


    return 0;
}