#include <kernel/devices/vesa.h>
#include <kernel/timer.h>
#include <stdio.h>

#include <graphics.h>

extern struct vbe_info_structure VBE_INFO;
extern struct vbe_mode_info_structure VBE_MODE_INFO;
extern uint8_t FONT;

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
