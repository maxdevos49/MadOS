/**
 * Graphics Library for madOS
 * Compatible with *VGA, VESA, and *GOP.
 * Designed for graphics modes with 32 bpp.
 * Supports both Double and Triple Buffering.
 * */
#ifndef __LIB_GRAPHICS_H
#define __LIB_GRAPHICS_H 1

#include <stdint.h>
#include <stddef.h>

#define CHAR_WIDTH 9
#define CHAR_HEIGHT 16

struct GRAPHICS_BUFFER
{
    uint32_t *framebuffer;
    int width;
    int height;
    int pitch;
    uint8_t bpp;
    uint8_t *font;
};

enum GRAPHICS_BUFFER_COUNT
{
    SINGLE,
    DOUBLE,
    TRIPLE
};

//Initializes graphic drawing
int graphics_init(struct GRAPHICS_BUFFER *buffer, enum GRAPHICS_BUFFER_COUNT buffer_count);
void graphics_uninit();

//Applies all updates to framebuffer
void swap_buffer();

void set_origin(int x, int y);

void fill();
void set_fill(uint32_t color);

void stroke();
void set_stroke(uint32_t color);

void set_line_width(uint32_t thickness);

void pixel(int x, int y, uint32_t color);
void draw_char(int x, int y, uint8_t c);
void draw_text(int x, int y, char *txt);


void move_to(int x, int y);
void line_to(int x, int y);

void rect(int x, int y, int w, int h);
void stroke_rect(int x, int y, int w, int h);
void fill_rect(int x, int y, int w, int h);
void clear_rect(int x, int y, int w, int h);

static inline uint64_t color32_to_color64(uint32_t clr)
{
    return (((uint64_t)clr) << 32) | (uint64_t)clr;
}

#endif