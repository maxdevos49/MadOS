#include <graphics.h>

#include <stdlib.h>
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

struct GRAPHICS_CONTEXT
{
    //readonly
    void *framebuffer;
    void *buffer0;
    void *buffer1;
    size_t buffer_size;
    int screen_width;
    int screen_height;
    int bpp;
    int pitch;
    uint8_t *font;
    enum GRAPHICS_BUFFER_COUNT buffer_count;

    //Always current drawing buffer
    void *buffer;
    enum GRAPHICS_CURRENT_BUFFER current_back_buffer;

    //Optimization rectangle maybe??
    int left_edit;
    int right_edit;
    int top_edit;
    int bottom_edit;

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

static struct GRAPHICS_CONTEXT ctx;

/**
 * Works so long as we have a valid framebuffer
 * */
static int graphics_error(char *str, int error_code)
{
    ctx.buffer = ctx.framebuffer;
    set_stroke(0x00ffffff);
    draw_text(ctx.screen_width / 2 - (strlen(str) * CHAR_WIDTH) / 2, ctx.screen_height / 2 - CHAR_HEIGHT / 2, str);

    while (1)
        ;
    return error_code;
}

int graphics_init(struct GRAPHICS_BUFFER *buffer, enum GRAPHICS_BUFFER_COUNT buffer_count)
{
    ctx.framebuffer = buffer->framebuffer;
    ctx.screen_width = buffer->width;
    ctx.screen_height = buffer->height;
    ctx.pitch = buffer->pitch;
    ctx.font = buffer->font;
    ctx.buffer_count = buffer_count;

    //Make sure its a supported bpp
    if (buffer->bpp != 32)
        return graphics_error("Invalid bpp. Excepted bpp == 32", 1); //Invalid bpp

    ctx.bpp = buffer->bpp;

    //Default buffer state
    ctx.buffer0 = NULL;
    ctx.buffer1 = NULL;
    ctx.current_back_buffer = FRAMEBUFFER;
    ctx.buffer = ctx.framebuffer;

    //Optimization rectangle init
    ctx.left_edit = 0;
    ctx.right_edit = 0;
    ctx.top_edit = 0;
    ctx.bottom_edit = 0;

    set_origin(0, 0);
    rect(0, 0, 0, 0);
    move_to(0, 0);
    set_fill(0x00000000);   //black
    set_stroke(0x00ffffff); //white
    set_line_width(4);      // 4 px

    //Buffer count init
    if (buffer_count == DOUBLE)
    {
        ctx.buffer0 = (void *)((uint64_t)malloc(ctx.screen_width * ctx.screen_height * sizeof(uint32_t)) & 0xfffffff0);

        if (ctx.buffer0 == NULL)
            return graphics_error("Malloc Error: buffer0 is null.", 4);

        ctx.buffer1 = NULL;
        ctx.current_back_buffer = BUFFER0;
        ctx.buffer = ctx.buffer0;
    }
    else if (buffer_count == TRIPLE)
    {
        ctx.buffer0 = malloc(ctx.screen_width * ctx.screen_height * sizeof(uint32_t));
        if (ctx.buffer0 == NULL)
            return graphics_error("Malloc Error: buffer0 is null.", 4);

        ctx.buffer1 = malloc(ctx.screen_width * ctx.screen_height * sizeof(uint32_t));
        if (ctx.buffer0 == NULL)
            return graphics_error("Malloc Error: buffer1 is null.", 4);

        ctx.current_back_buffer = BUFFER0;
        ctx.buffer = ctx.buffer0;
    }
    else if (buffer_count != SINGLE)
    {
        return graphics_error("Invalid buffer Count", 2);
    }

    clear_rect(0, 0, ctx.screen_width, ctx.screen_height);

    return 0;
}

void swap_buffer()
{
    //If single buffering there is no need to swap anything
    if (ctx.current_back_buffer == FRAMEBUFFER)
        return;

    uint64_t *f_offset = (uint64_t *)ctx.framebuffer;
    uint64_t *b_offset = (uint64_t *)ctx.buffer;

    for (int i = 0; i < (ctx.screen_height * ctx.screen_width) / 2; i++)
        f_offset[i] = b_offset[i];

    //If double buffering then there is no back buffer swap
    if (ctx.buffer_count == DOUBLE)
        return;

    //Swap target buffers
    if (ctx.current_back_buffer == BUFFER0)
    {
        ctx.buffer = ctx.buffer1;
        ctx.current_back_buffer = BUFFER1;
    }
    else if (ctx.current_back_buffer == BUFFER1)
    {
        ctx.buffer = ctx.buffer0;
        ctx.current_back_buffer = BUFFER0;
    }
}

void set_origin(int x, int y)
{
    ctx.origin_x = x;
    ctx.origin_y = y;
}

void fill()
{
    if (ctx.mode == RECT)
    {
        int left = ctx.x + ctx.origin_x;
        int right = ctx.x + ctx.w + ctx.origin_x;
        int top = ctx.y + ctx.origin_y;
        int bottom = ctx.y + ctx.h + ctx.origin_y;

        int width = ctx.screen_width;
        int height = ctx.screen_height;

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
            uint32_t *offset = (i * ctx.pitch + ctx.buffer);

            for (int j = left; j < right; j++)
                offset[j] = ctx.fill_32;
        }
    }
}

void set_fill(uint32_t color)
{
    ctx.fill_64 = color32_to_color64(color);
    ctx.fill_32 = color;
}

static void render_line(int x0, int y0, int x1, int y1)
{

    int dx = g_abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -g_abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy; /* error value e_xy */

    while (1)
    {
        pixel(x0, y0, ctx.stroke_32);

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

void stroke()
{
    int left = ctx.x;
    int top = ctx.y;
    int right = left + ctx.w;
    int bottom = top + ctx.h;

    if (ctx.mode == RECT)
    {
        move_to(left, top);
        line_to(right, top);
        stroke();
        line_to(right, bottom);
        stroke();
        line_to(left, bottom);
        stroke();
        line_to(left, top);
        stroke();
        ctx.mode = RECT;
        move_to(left, top);
    }
    else if (ctx.mode == LINE)
    {
        int x0 = ctx.x;
        int y0 = ctx.y;

        int x1 = ctx.line_x;
        int y1 = ctx.line_y;
        for (int i = -ctx.line_width / 2; i < ctx.line_width / 2; i++)
        {
            render_line(x0 + i, y0 + i, x1 + i, y1 + i);
        }
    }
}

void set_stroke(uint32_t color)
{
    ctx.stroke_64 = color32_to_color64(color);
    ctx.stroke_32 = color;
}

void set_line_width(uint32_t thickness)
{
    ctx.line_width = thickness;
}

void move_to(int x, int y)
{
    ctx.x = x;
    ctx.y = y;
    ctx.line_x = x;
    ctx.line_y = y;
    ctx.mode = NONE;
}

void line_to(int x, int y)
{
    ctx.x = ctx.line_x;
    ctx.y = ctx.line_y;
    ctx.line_x = x;
    ctx.line_y = y;
    ctx.mode = LINE;
    stroke();
}

void rect(int x, int y, int w, int h)
{
    ctx.x = x;
    ctx.y = y;
    ctx.w = w;
    ctx.h = h;
    ctx.mode = RECT;
}

void stroke_rect(int x, int y, int w, int h)
{
    rect(x, y, w, h);
    stroke();
}

void fill_rect(int x, int y, int w, int h)
{
    rect(x, y, w, h);
    fill();
}

void clear_rect(int x, int y, int w, int h)
{
    uint32_t color = ctx.fill_32;
    set_fill(0x00000000);
    fill_rect(x - ctx.origin_x, y - ctx.origin_y, w, h);
    set_fill(color);
}

void draw_char(int x, int y, uint8_t c)
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
    if (right > ctx.screen_width + CHAR_WIDTH || bottom > ctx.screen_height + CHAR_HEIGHT)
        return;

    uint8_t *glyph = (uint8_t *)((uint64_t)ctx.font + (uint64_t)c * (uint64_t)16);

    for (cy = 0; cy < 16; cy++)
    {
        uint32_t *offset = (uint32_t *)((top + cy) * ctx.pitch + (uint64_t)ctx.buffer);

        for (cx = 0; cx < 8; cx++)
        {
            if (left + CHAR_WIDTH - cx + 1 > ctx.screen_width)
                continue;

            if (glyph[cy] & mask[cx])
                offset[left - cx + CHAR_WIDTH] = ctx.stroke_32;
        }
    }
}

void draw_text(int x, int y, char *txt)
{
    int sx = x + ctx.origin_x;
    int sy = y + ctx.origin_y;

    int i = 0;
    int offset = 0;
    while (txt[i] != NULL)
    {
        draw_char(sx + offset, sy, txt[i]);
        i++;
        offset += CHAR_WIDTH;
    }
}

void pixel(int x, int y, uint32_t color)
{
    int sx = x + ctx.origin_x;
    int sy = y + ctx.origin_y;

    if (sx < 0 || sx > ctx.screen_width)
        return;

    if (sy < 0 || sy > ctx.screen_height)
        return;

    *((uint32_t *)(sy * ctx.pitch + (sx * 4) + (uint64_t)ctx.buffer)) = color;
}