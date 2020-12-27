#ifndef __GRAPHICS_TGA_H
#define __GRAPHICS_TGA_H 1

#include <stdint.h>

//From https://github.com/nothotscott/VisualOS/commit/dd92f3bb6612f54d620768d2f990ec9159c93d49
struct TGA_HEADER
{
    uint8_t id_length;
    uint8_t color_map_type;
    uint8_t image_type;
    uint16_t color_map_index;
    uint16_t color_map_length;
    uint8_t depth;
    uint16_t x_org; // x origin
    uint16_t y_org; // y origin
    uint16_t width;
    uint16_t height;
    uint8_t bbp; // bits per pixel
    uint8_t descriptor;
} __attribute__((__packed__));

struct TGA_IMAGE
{
    struct TGA_HEADER *header_ptr;
    void *buffer;
};

void draw_tga(void *frame_buffer, struct TGA_IMAGE *img, int xoff, int yoff);

#endif