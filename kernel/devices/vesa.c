#include <kernel/devices/vesa.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>

struct VBE_info_structure
{
    char signature[4];     // must be "VESA" to indicate valid VBE support
    uint16_t version;      // VBE version; high byte is major version, low byte is minor version
    uint32_t oem;          // segment:offset pointer to OEM
    uint32_t capabilities; // bitfield that describes card capabilities
    uint32_t video_modes;  // segment:offset pointer to list of supported video modes
    uint16_t video_memory; // amount of video memory in 64KB blocks
    uint16_t software_rev; // software revision
    uint32_t vendor;       // segment:offset to card vendor string
    uint32_t product_name; // segment:offset to card model name
    uint32_t product_rev;  // segment:offset pointer to product revision
    char reserved[222];    // reserved for future expansion
    char oem_data[256];    // OEM BIOSes store their strings in this area
} __attribute__((packed));
struct VBE_mode_info_structure
{
    uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;     // deprecated
    uint8_t window_b;     // deprecated
    uint16_t granularity; // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;        // number of bytes per horizontal line
    uint16_t width;        // width in pixels
    uint16_t height;       // height in pixels
    uint8_t w_char;        // unused...
    uint8_t y_char;        // ...
    uint8_t planes;
    uint8_t bpp;   // bits per pixel in this mode
    uint8_t banks; // deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__((packed));

extern struct VBE_info_structure VBE_INFO;
extern struct VBE_mode_info_structure VBE_MODE_INFO;
extern uint8_t FONT;

static int previous_cursor_position;
static int cursor_active;
static void set_cursor(TTY_CONFIG *config);
static void put_char(TTY_CONFIG *config, char c, uint32_t position);
static void clear(TTY_CONFIG *config);
static void write(TTY_CONFIG *config, const char *data, size_t size);
static void scroll(TTY_CONFIG *config);

void VESA_install()
{
    printf("Initializing VESA\n");

    struct GRAPHICS_BUFFER buffer;
    printf("\tFramebuffer: %x\n", VBE_MODE_INFO.framebuffer);
    buffer.framebuffer = (void *)(long)VBE_MODE_INFO.framebuffer;

    printf("\tWidth: %d\n", VBE_MODE_INFO.width);
    buffer.width = VBE_MODE_INFO.width;

    printf("\tHeight: %d\n", VBE_MODE_INFO.height);
    buffer.height = VBE_MODE_INFO.height;

    printf("\tBPP: %d\n", VBE_MODE_INFO.bpp);
    buffer.bpp = VBE_MODE_INFO.bpp;

    printf("\tPitch: %d\n", VBE_MODE_INFO.pitch);
    buffer.pitch = VBE_MODE_INFO.pitch;
    buffer.font = &FONT;

    graphics_init(&buffer);
}

void VESA_tty_init(TTY_CONFIG *config, GRAPHICS_CONTEXT *ctx)
{
    config->char_width = get_ctx_width(ctx) / CHAR_WIDTH;
    config->char_height = get_ctx_height(ctx) / CHAR_HEIGHT;
    previous_cursor_position = 0;
    cursor_active = 1;
    config->cursor_position = 0;
    config->foreground_theme = 0x0000ff00;
    config->background_theme = 0x00000000;

    config->set_cursor = set_cursor;
    config->put_char = put_char;
    config->clear = clear;
    config->write = write;
    config->scroll = scroll;
    config->tty_ctx = ctx;
}

static void invert_char(TTY_CONFIG *config, int x, int y)
{
    cursor_active = !cursor_active; //Invert
    int left = x * CHAR_WIDTH;
    int right = left + CHAR_WIDTH;
    int top = y * CHAR_HEIGHT;
    int bottom = top + CHAR_HEIGHT;

    for (int i = top; i < bottom; i++)
    {
        uint32_t *offset = (uint32_t *)(i * get_ctx_pitch(config->tty_ctx) + (uint64_t)get_backbuffer0(config->tty_ctx));

        for (int j = left; j < right; j++)
        {
            //invert
            if (offset[j] == config->background_theme)
                offset[j] = config->foreground_theme;
            else
                offset[j] = config->background_theme;
        }
    }
}

static void set_cursor(TTY_CONFIG *config)
{
    int x, y;

   if (cursor_active == 1)
    {
        x = previous_cursor_position % config->char_width;
        y = previous_cursor_position / config->char_width;
        invert_char(config, x, y);
    }

    //invert
    x = config->cursor_position % config->char_width;
    y = config->cursor_position / config->char_width;

    invert_char(config, x, y);

 

    swap_buffer(config->tty_ctx);
    previous_cursor_position = config->cursor_position;
}

static void put_char(TTY_CONFIG *config, char c, uint32_t position)
{
    int x, y;

    x = position % config->char_width;
    y = position / config->char_width;

    //Background
    set_fill(config->tty_ctx, config->background_theme);
    fill_rect(config->tty_ctx, x * CHAR_WIDTH, y * CHAR_HEIGHT, CHAR_WIDTH, CHAR_HEIGHT);

    //Character
    set_stroke(config->tty_ctx, config->foreground_theme);
    draw_char(config->tty_ctx, x * CHAR_WIDTH, y * CHAR_HEIGHT, c);

    swap_buffer(config->tty_ctx);
}

static void clear(TTY_CONFIG *config)
{
    set_fill(config->tty_ctx, config->background_theme);
    fill_rect(config->tty_ctx, 0, 0, get_ctx_width(config->tty_ctx), get_ctx_height(config->tty_ctx));
}

static void write(TTY_CONFIG *config, const char *data, size_t size)
{

    start_batch(config->tty_ctx, 6996);

    uint64_t i;
    for (i = 0; i < size; i++)
        TTY_putchar(*(data + i));

    end_batch(config->tty_ctx, 6996);
}

static void scroll(TTY_CONFIG *config)
{

    int x, y;

    x = config->cursor_position % config->char_width;
    y = config->cursor_position / config->char_width;

    invert_char(config, x, y);

    uint32_t length = get_ctx_height(config->tty_ctx) * get_ctx_pitch(config->tty_ctx);
    void *dst_ptr = get_backbuffer0(config->tty_ctx);
    void *src_ptr = dst_ptr + (CHAR_HEIGHT * get_ctx_pitch(config->tty_ctx));

    memmove(dst_ptr, src_ptr, length);

    //Clear bottom line
    set_fill(config->tty_ctx, config->background_theme);
    fill_rect(config->tty_ctx, 0, (config->char_height - 1) * CHAR_HEIGHT, get_ctx_width(config->tty_ctx), CHAR_HEIGHT);

    swap_buffer(config->tty_ctx);
}
