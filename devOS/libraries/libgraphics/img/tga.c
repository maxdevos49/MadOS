#include "tga.h"
#include "graphics.h"
#include "../opaque/context.h"

#include <stdint.h>
#include <stddef.h>

void draw_tga(GRAPHICS_CONTEXT *ctx, struct TGA_IMAGE *img, int x_offset, int y_offset)
{

    uint32_t *img_buffer = (uint32_t *)img->buffer;
    uint32_t img_pitch = img->header_ptr->width * img->header_ptr->bpp;

    int top = y_offset + ctx->origin_x;
    int bottom = top + img->header_ptr->height;
    int left = x_offset + ctx->origin_y;
    int right = left + img->header_ptr->width;

    int ii, bi, ij, bj;
    for (bi = top, ii = 0; bi < bottom; bi++, ii++)
    {
        uint32_t *b_offset = (uint32_t *)(ctx->pitch * bi + (uint64_t)ctx->buffer);
        uint32_t *i_offset = (uint32_t *)(img_pitch * ii + (uint64_t)img_buffer);

        for (bj = left, ij = 0; bj < right; bj++, ij++)
        {
            if (i_offset != 0)
                b_offset[bj] = i_offset[ij];
        }
    }
}