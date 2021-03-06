#ifndef __GRAPHICS_CONTEXT_H
#define __GRAPHICS_CONTEXT_H 1

#include <stdint.h>
#include <stddef.h>


enum GRAPHICS_CURRENT_BUFFER
{
    FRAMEBUFFER,
    BUFFER0,
    BUFFER1
};

enum GRAPHICS_MODE
{
    NONE,
    RECT,
    ELLIPSE,
    TEXT,
    LINE
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

    int batch_id;

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

#endif