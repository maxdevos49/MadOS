#include "graphics.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "opaque/context.h"

static int g_abs(int n)
{
    if (n >= 0)
        return n;

    return n * -1;
}

static struct GRAPHICS_BUFFER _buffer;

int graphics_init(struct GRAPHICS_BUFFER *buffer)
{
    if (buffer == NULL)
        return 1;

    if ((_buffer.bpp = buffer->bpp) != 32)
        return 2;

    if ((_buffer.font = buffer->font) == NULL)
        return 3;

    if ((_buffer.framebuffer = buffer->framebuffer) == NULL)
        return 4;

    if ((_buffer.height = buffer->height) <= 0)
        return 5;

    if ((_buffer.width = buffer->width) <= 0)
        return 6;

    if ((_buffer.pitch = buffer->pitch) <= 0)
        return 7;

    _buffer.valid = 1;

    return 0;
}

int graphics_uninit()
{
    if (_buffer.valid == 0)
        return 1;

    _buffer.valid = 0;
    _buffer.framebuffer = NULL;
    _buffer.font = NULL;
    _buffer.bpp = 0;
    _buffer.height = 0;
    _buffer.width = 0;
    _buffer.pitch = 0;

    return 0;
}

GRAPHICS_CONTEXT *get_graphics_ctx(enum GRAPHICS_BUFFER_COUNT buffer_count, int x, int y, int width, int height)
{
    GRAPHICS_CONTEXT *ctx = (struct __graphics_context *)malloc(sizeof(struct __graphics_context));

    if (ctx == NULL)
    {
        print_error(ctx, "CTX malloc error", 1);
        return NULL;
    }

    if (_buffer.valid == 0)
    {
        print_error(ctx, "Invalid Buffer", 1);
        return NULL;
    }

    ctx->buffer_count = buffer_count;

    ctx->x_offset = x;
    ctx->y_offset = y;
    ctx->ctx_width = width;
    ctx->ctx_height = height;
    ctx->pitch = width * sizeof(uint32_t);

    //Default buffer state
    ctx->buffer0 = NULL;
    ctx->buffer1 = NULL;
    ctx->current_back_buffer = FRAMEBUFFER;
    ctx->buffer = _buffer.framebuffer;
    ctx->buffer_size = width * height * sizeof(uint32_t);

    set_origin(ctx, 0, 0);
    rect(ctx, 0, 0, 0, 0);
    move_to(ctx, 0, 0);
    set_fill(ctx, 0x00000000);   //black
    set_stroke(ctx, 0x00ffffff); //white
    set_line_width(ctx, 4);      // 4 px

    //Buffer count init
    if (buffer_count == DOUBLE)
    {
        ctx->buffer0 = malloc(ctx->ctx_width * ctx->ctx_height * sizeof(uint32_t));

        if (ctx->buffer0 == NULL)
        {
            print_error(ctx, "Malloc Error: buffer0 is null.", 4);
            return NULL;
        }

        ctx->buffer1 = NULL;
        ctx->current_back_buffer = BUFFER0;
        ctx->buffer = ctx->buffer0;
    }
    else if (buffer_count == TRIPLE)
    {
        ctx->buffer0 = malloc(ctx->ctx_width * ctx->ctx_height * sizeof(uint32_t));
        if (ctx->buffer0 == NULL)
        {
            print_error(ctx, "Malloc Error: buffer0 is null.", 4);
            return NULL;
        }

        ctx->buffer1 = malloc(ctx->ctx_width * ctx->ctx_height * sizeof(uint32_t));
        if (ctx->buffer0 == NULL)
        {
            print_error(ctx, "Malloc Error: buffer1 is null.", 4);
            return NULL;
        }

        ctx->current_back_buffer = BUFFER0;
        ctx->buffer = ctx->buffer0;
    }
    else if (buffer_count != SINGLE)
    {
        print_error(ctx, "Invalid buffer Count", 2);
        return NULL;
    }

    // clear_rect(ctx, 0, 0, ctx->ctx_width, ctx->ctx_height);
    // swap_buffer(ctx);

    return ctx;
}

int destroy_graphics_ctx(GRAPHICS_CONTEXT *ctx)
{
    if (ctx == NULL)
        return 1;

    if (ctx->buffer_count == DOUBLE)
    {
        free(ctx->buffer0);
    }
    else if (ctx->buffer_count == TRIPLE)
    {
        free(ctx->buffer0);
        free(ctx->buffer1);
    }

    free(ctx);

    return 0;
}

int print_error(GRAPHICS_CONTEXT *ctx, char *str, int error_code)
{
    ctx->buffer = _buffer.framebuffer;
    ctx->current_back_buffer = FRAMEBUFFER;

    set_stroke(ctx, 0x00ffffff);
    draw_text(ctx, _buffer.width / 2 - (strlen(str) * CHAR_WIDTH) / 2, _buffer.height / 2 - CHAR_HEIGHT / 2, str);
    printf("%s\n", str);

    return error_code;
}

void swap_buffer(GRAPHICS_CONTEXT *ctx)
{
    if (ctx->batch_id != NULL)
        return;

    
    //If single buffering there is no need to swap anything
    if (ctx->current_back_buffer == FRAMEBUFFER)
        return;

    int left = ctx->x_offset;
    int right = ctx->ctx_width + left;
    int top = ctx->y_offset;
    int bottom = ctx->ctx_height + top;

    int fi, bi, fj, bj;
    for (fi = top, bi = 0; fi < bottom; fi++, bi++)
    {
        uint32_t *f_offset = (uint32_t *)(_buffer.pitch * fi + (uint64_t)_buffer.framebuffer);
        uint32_t *b_offset = (uint32_t *)(ctx->pitch * bi + (uint64_t)ctx->buffer);

        for (fj = left, bj = 0; fj < right; fj++, bj++)
            f_offset[fj] = b_offset[bj];
    }

    //If double buffering then there is no back buffer swap
    if (ctx->buffer_count == DOUBLE)
        return;

    //Swap target buffers
    if (ctx->current_back_buffer == BUFFER0)
    {
        ctx->buffer = ctx->buffer1;
        ctx->current_back_buffer = BUFFER1;
    }
    else if (ctx->current_back_buffer == BUFFER1)
    {
        ctx->buffer = ctx->buffer0;
        ctx->current_back_buffer = BUFFER0;
    }
}

void set_origin(GRAPHICS_CONTEXT *ctx, int x, int y)
{
    ctx->origin_x = x;
    ctx->origin_y = y;
}

void fill(GRAPHICS_CONTEXT *ctx)
{
    if (ctx->mode == RECT)
    {
        int left = ctx->x + ctx->origin_x;
        int right = left + ctx->w;
        int top = ctx->y + ctx->origin_y;
        int bottom = top + ctx->h;

        int width = ctx->ctx_width;
        int height = ctx->ctx_height;

        if (left < 0)
            left = 0;
        if (left > width)
            left = width;

        if (right < 0)
            right = 0;
        if (right > width)
            right = width;

        if (top < 0)
            top = 0;
        if (top > height)
            top = height;

        if (bottom < 0)
            bottom = 0;
        if (bottom > height)
            bottom = height;

        for (int i = top; i < bottom; i++)
        {
            uint32_t *offset = (uint32_t *)(i * ctx->pitch + (uint64_t)ctx->buffer);

            for (int j = left; j < right; j++)
                offset[j] = ctx->fill_32;
        }
    }
}

void set_fill(GRAPHICS_CONTEXT *ctx, uint32_t color)
{
    ctx->fill_64 = color32_to_color64(color);
    ctx->fill_32 = color;
}

static void render_line(GRAPHICS_CONTEXT *ctx, int x0, int y0, int x1, int y1)
{

    int dx = g_abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -g_abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy; /* error value e_xy */

    while (1)
    {
        pixel(ctx, x0, y0, ctx->stroke_32);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= dy)
        { /* e_xy+e_x > 0 */
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        { /* e_xy+e_y < 0 */
            err += dx;
            y0 += sy;
        }
    }
}

void stroke(GRAPHICS_CONTEXT *ctx)
{

    if (ctx->mode == RECT)
    {
        int left = ctx->x;
        int top = ctx->y;
        int right = left + ctx->w;
        int bottom = top + ctx->h;

        move_to(ctx, left, top);
        line_to(ctx, right, top);
        stroke(ctx);
        line_to(ctx, right, bottom);
        stroke(ctx);
        line_to(ctx, left, bottom);
        stroke(ctx);
        line_to(ctx, left, top);
        stroke(ctx);
        ctx->mode = RECT;
        move_to(ctx, left, top);
    }
    else if (ctx->mode == LINE)
    {
        int x0 = ctx->x;
        int y0 = ctx->y;

        int x1 = ctx->line_x;
        int y1 = ctx->line_y;

        int left_offset = 0 - ctx->line_width / 2;                //TODO needs work
        int right_offset = ctx->line_width - ctx->line_width / 2; //ctx->line_width / 2;

        for (int i = left_offset; i < right_offset; i++)
        {
            render_line(ctx, x0 + i, y0 + i, x1 + i, y1 + i);
        }
    }
}

void set_stroke(GRAPHICS_CONTEXT *ctx, uint32_t color)
{
    ctx->stroke_64 = color32_to_color64(color);
    ctx->stroke_32 = color;
}

void set_line_width(GRAPHICS_CONTEXT *ctx, uint32_t thickness)
{
    ctx->line_width = thickness;
}

void move_to(GRAPHICS_CONTEXT *ctx, int x, int y)
{
    ctx->x = x;
    ctx->y = y;
    ctx->line_x = x;
    ctx->line_y = y;
    ctx->mode = NONE;
}

void line_to(GRAPHICS_CONTEXT *ctx, int x, int y)
{
    ctx->x = ctx->line_x;
    ctx->y = ctx->line_y;
    ctx->line_x = x;
    ctx->line_y = y;
    ctx->mode = LINE;
    stroke(ctx);
}

void rect(GRAPHICS_CONTEXT *ctx, int x, int y, int w, int h)
{
    ctx->x = x;
    ctx->y = y;
    ctx->w = w;
    ctx->h = h;
    ctx->mode = RECT;
}

void stroke_rect(GRAPHICS_CONTEXT *ctx, int x, int y, int w, int h)
{
    rect(ctx, x, y, w, h);
    stroke(ctx);
}

void fill_rect(GRAPHICS_CONTEXT *ctx, int x, int y, int w, int h)
{
    rect(ctx, x, y, w, h);
    fill(ctx);
}

void clear_rect(GRAPHICS_CONTEXT *ctx, int x, int y, int w, int h)
{
    uint32_t color = ctx->fill_32;
    set_fill(ctx, 0x00000000);
    fill_rect(ctx, x - ctx->origin_x, y - ctx->origin_y, w, h);
    set_fill(ctx, color);
}

void draw_char(GRAPHICS_CONTEXT *ctx, int x, int y, uint8_t c)
{
    int cx, cy;
    int mask[8] = {1, 2, 4, 8, 16, 32, 64, 128};

    int left = x;
    int right = x + CHAR_WIDTH;
    int top = y;
    int bottom = y + CHAR_HEIGHT;

    //CULL top and left
    if (left < -CHAR_WIDTH || top < -CHAR_HEIGHT)
        return;

    //CULL bottom and right
    if (right > ctx->ctx_width + CHAR_WIDTH || bottom > ctx->ctx_height + CHAR_HEIGHT)
        return;

    uint8_t *glyph = (uint8_t *)((uint64_t)_buffer.font + (uint64_t)c * (uint64_t)16);

    for (cy = 0; cy < 16; cy++)
    {
        uint32_t *offset = (uint32_t *)((top + cy) * ctx->pitch + (uint64_t)ctx->buffer);

        for (cx = 0; cx < 8; cx++)
        {
            if (left + CHAR_WIDTH - cx + 1 > ctx->ctx_width)
                continue;

            if (glyph[cy] & mask[cx])
                offset[left - cx + CHAR_WIDTH] = ctx->stroke_32;
        }
    }
}

void draw_text(GRAPHICS_CONTEXT *ctx, int x, int y, char *txt)
{
    int sx = x + ctx->origin_x;
    int sy = y + ctx->origin_y;

    int i = 0;
    int offset = 0;
    while (txt[i] != '\0')
    {
        draw_char(ctx, sx + offset, sy, txt[i]);
        i++;
        offset += CHAR_WIDTH;
    }
}

void pixel(GRAPHICS_CONTEXT *ctx, int x, int y, uint32_t color)
{
    int sx = x + ctx->origin_x;
    int sy = y + ctx->origin_y;

    if (sx < 0 || sx > ctx->ctx_width)
        return;

    if (sy < 0 || sy > ctx->ctx_height)
        return;

    *((uint32_t *)(sy * ctx->pitch + (sx * 4) + (uint64_t)ctx->buffer)) = color;
}

int start_batch(GRAPHICS_CONTEXT *ctx, int id)
{
    if (ctx->batch_id != NULL)
        return 1; //batch id already set

    ctx->batch_id = id;

    return 0; //Success
}

int end_batch(GRAPHICS_CONTEXT *ctx, int id)
{
    if (ctx->batch_id != id)
        return 1; //Batch id is invalid

    ctx->batch_id = NULL;

    swap_buffer(ctx);

    return 0; //Success
}

uint32_t *get_backbuffer0(GRAPHICS_CONTEXT *ctx)
{
    return ctx->buffer0;
}

uint32_t *get_backbuffer1(GRAPHICS_CONTEXT *ctx)
{
    return ctx->buffer1;
}

uint32_t get_screen_width()
{
    return _buffer.width;
}

uint32_t get_screen_height()
{
    return _buffer.height;
}

uint32_t get_ctx_width(GRAPHICS_CONTEXT *ctx)
{
    return ctx->ctx_width;
}

uint32_t get_ctx_height(GRAPHICS_CONTEXT *ctx)
{
    return ctx->ctx_height;
}

uint32_t get_ctx_pitch(GRAPHICS_CONTEXT *ctx)
{
    return ctx->pitch;
}