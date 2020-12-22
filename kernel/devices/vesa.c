#include <kernel/devices/vesa.h>
#include <kernel/timer.h>
#include <stdio.h>
#include <graphics.h>

extern struct vbe_info_structure VBE_INFO;
extern struct vbe_mode_info_structure VBE_MODE_INFO;

extern uint8_t FONT;

void VESA_install(struct GRAPHICS_BUFFER *buffer)
{
    buffer->framebuffer = (void *)(long)VBE_MODE_INFO.framebuffer;
    buffer->width = VBE_MODE_INFO.width;
    buffer->height = VBE_MODE_INFO.height;
    buffer->bpp = VBE_MODE_INFO.bpp;
    buffer->pitch = VBE_MODE_INFO.pitch;
    buffer->font = &FONT;
}
