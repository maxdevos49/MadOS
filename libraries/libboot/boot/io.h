#ifndef __IO_H
#define __IO_H 1

static inline void outb(unsigned short int port, unsigned char val)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(val), "Nd"(port));
}
static inline unsigned char inb(unsigned short int port)
{
    unsigned char return_val;

    asm volatile("inb %1, %0"
                 : "=a"(return_val)
                 : "Nd"(port));

    return return_val;
}

static inline void outw(unsigned short int port, unsigned short int val)
{
    asm volatile("outw %0, %1"
                 :
                 : "a"(val), "Nd"(port));
}
static inline unsigned short int inw(unsigned short int port)
{
    unsigned short int return_val;

    asm volatile("inw %1, %0"
                 : "=a"(return_val)
                 : "Nd"(port));

    return return_val;
}

static inline void outl(unsigned short int port, unsigned int val)
{
    asm volatile("outl %0, %1"
                 :
                 : "a"(val), "Nd"(port));
}
static inline unsigned int inl(unsigned short int port)
{
    unsigned int return_val;

    asm volatile("inl %1, %0"
                 : "=a"(return_val)
                 : "Nd"(port));

    return return_val;
}

static inline void io_wait(void)
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile("outb %%al, $0x80"
                 :
                 : "a"(0));
    /* %%al instead of %0 makes no difference.  TODO: does the register need to be zeroed? */
}

#endif