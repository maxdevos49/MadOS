#include "graphics.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

enum GRAPHICS_MODE
{
    NONE,
    RECT,
    ELLIPSE,
    TEXT,
    LINE
};
enum GRAPHICS_CURRENT_BUFFER
{
    FRAMEBUFFER,
    BUFFER0,
    BUFFER1
};
struct __graphics_context
{
    int x_offset;
    int y_offset;
    int ctx_width;
    int ctx_height;
    uint32_t pitch;

    void *buffer0;
    void *buffer1;
    size_t buffer_size;

    enum GRAPHICS_BUFFER_COUNT buffer_count;

    //Always current drawing buffer
    void *buffer;
    enum GRAPHICS_CURRENT_BUFFER current_back_buffer;

    //draw origin
    int origin_x;
    int origin_y;

    //Draw details
    int x;
    int y;
    int w;
    int h;

    //Line end
    int line_x;
    int line_y;
    int line_width;

    //Line color in different sizes for when needed
    uint64_t stroke_64;
    uint32_t stroke_32;

    //Fill color in different sizes for when needed
    uint64_t fill_64;
    uint32_t fill_32;

    enum GRAPHICS_MODE mode;
};

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
    printf("Gettings Graphics context\n");
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
    ctx->buffer_size = width * height;

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

    clear_rect(ctx, 0, 0, ctx->ctx_width, ctx->ctx_height);
    swap_buffer(ctx);

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
    //If single buffering there is no need to swap anything
    if (ctx->current_back_buffer == FRAMEBUFFER)
        return;

    int x, y, w, h;
    x = ctx->x_offset;
    y = ctx->y_offset;
    w = ctx->ctx_width + x;
    h = ctx->ctx_height + y;

    printf("X:%d Y:%d W:%d H:%d\n", x, y, w, h);

    for (int i = y; i < h; i++)
    {
        uint32_t *f_offset = (uint32_t *)((uint64_t)_buffer.framebuffer + (_buffer.width * i));
        uint32_t *b_offset = (uint32_t *)((uint64_t)ctx->buffer + (_buffer.width * i));

        for (int j = x; j < w; j++)
            f_offset[j] = b_offset[j];
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
    // printf("Fill(): 0x%x\n", fill);
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
            uint32_t *offset = (uint32_t *)((uint64_t)i * (uint64_t)_buffer.width + (uint64_t)ctx->buffer);

            for (int j = left; j < right; j++)
                offset[j] = ctx->fill_32;
        }

        // printf("L:%d R:%d T:%d B:%d W:%d H:%d M:%d\n", left, right, top, bottom, width, height, ctx->mode);
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
    int left = ctx->x;
    int top = ctx->y;
    int right = left + ctx->w;
    int bottom = top + ctx->h;

    if (ctx->mode == RECT)
    {
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
        for (int i = -ctx->line_width / 2; i < ctx->line_width / 2; i++)
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
    while (txt[i] != NULL)
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