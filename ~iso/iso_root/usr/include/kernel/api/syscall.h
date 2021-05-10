#ifndef __KERNEL_API_SYSCALL_H
#define __KERNEL_API_SYSCALL_H 1

void SYSCALL_malloc();
void SYSCALL_free();

// void SYSCALL_mmap();

void SYSCALL_open();
void SYSCALL_write();
void SYSCALL_read();
void SYSCALL_seek();
void SYSCALL_close();

void SYSCALL_fork();
void SYSCALL_yield();
void SYSCALL_exec();
void SYSCALL_exit();
void SYSCALL_kill();

void SYSCALL_beep();

void SYSCALL_reboot();
void SYSCALL_shutdown();
void SYSCALL_halt();


#endif