#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define BUFSIZ 512
#define EOF (-1)
#define FILENAME_MAX (50)
#define FOPEN_MAX (20)
#define L_tmpname (10)
#define NULL (0)
#define TMP_MAX (50)

struct _io_buffer //TODO figure out definition
{
    int level;             /* fill/empty level of buffer */
    unsigned flags;        /* File status flags          */
    char fd;               /* File descriptor            */
    unsigned char hold;    /* Ungetc char if no buffer   */
    int bsize;             /* Buffer size                */
    unsigned char *buffer; /* Data transfer buffer       */
    unsigned char *curp;   /* Current active pointer     */
    unsigned istemp;       /* Temporary file indicator   */
    short token;           /* Used for validity checking */
};

typedef struct _io_buffer FILE;
typedef struct _fpos fpos_t;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int fclose(FILE *stream);
int clearerr(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
int fflush(FILE *stream);
int fgetpos(FILE *stream, fpos_t *pos);
FILE *fopen(const char *filename, const char *mode);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
FILE *freopen(const char *filename, const char *mode, FILE *stream);
int fseek(FILE *stream, long int offset, int whence);
int fsetpos(FILE *stream, const fpos_t *pos);
long int ftell(FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
int remove(const char *filename);
int rename(const char *old_filename, const char *new_filename);
void rewind(FILE *stream);
void setbuf(FILE *stream, char *buffer);
int setvbuf(FILE *stream, char *buffer, int mode, size_t size);
FILE *tmpfile(void);
char *fprintf(FILE *stream, const char *format, ...);
int printf(const char *restrict , ...);
int sprintf(char *str, const char *restrict , ...);
int vsprintf(char *buffer, const char *format, va_list args);
int vfprintf(FILE *stream, const char *format, va_list arg);
int vprintf(const char *format, va_list arg);
int fscanf(FILE *stream, const char *format, ...);
int scanf(const char *format, ...);
int sscanf(const char *str, const char *format, ...);
int fgetc(FILE *stream);
char *fgets(char *str, int n, FILE *stream);
int fputc(int c, FILE *stream);
int fputs(const char *str, FILE *stream);
int getc(FILE *stream);
int getchar(void);
char *gets(char *str);
int putc(int c, FILE *stream);
int putchar(int c);
int puts(const char *);
int ungetc(int c, FILE *stream);
void perror(const char *str);

#endif