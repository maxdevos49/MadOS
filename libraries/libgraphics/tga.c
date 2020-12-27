#include "tga.h"
#include "graphics.h"

#include <stdint.h>
#include <stddef.h>

void draw_tga(GRAPHICS_CONTEXT *ctx, struct TGA_IMAGE *img, int xoff, int yoff)
{

    uint32_t *img_ptr = (uint32_t *)img->buffer;
    uint32_t height = img->header_ptr->height;
    uint32_t width = img->header_ptr->width;

    for (size_t dy = 0; dy < height; dy++)
    {
        for (size_t dx = 0; dx < width; dx++)
        {
            size_t offset = dx + (height * dy);
            uint32_t color = *(img_ptr + offset);
            size_t x = dx + xoff;
            size_t y = dy + yoff;
            *(uint *)(pixel_ptr + x + (y * frame_buffer->ppsl)) = color; //TODO
        }
    }