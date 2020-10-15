#include <stdint.h>

#ifndef __IDT__
#define __IDT__

typedef struct IDT64
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t types_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} IDT64_t;

void init_IDT();

// This was a pain in the ass. Turns out function pointers are a 
// definition + declaration so a extern is required to make it 
// only a declaration
extern void (*main_keyboard_handler)(uint8_t, char); 

#endif
