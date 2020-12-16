#include <kernel/devices/vesa.h>
#include <stdio.h>

extern struct vbe_info_structure VBE_INFO;
extern struct vbe_mode_info_structure VBE_MODE_INFO;

static uint32_t *framebuffer;
static uint32_t width;
static uint32_t height;
static uint8_t bpp;
static uint32_t pitch;

void VESA_install()
{
    // printf("Installing VESA\n");
    // printf("\tSignature: %s\n", VBE_INFO.signature);
    // printf("\tOEM: %s\n", VBE_INFO.oem);
    // printf("\tCapabilities: %x\n", VBE_INFO.capabilities);
    // printf("\tVideo Modes: %x\n", VBE_INFO.video_modes);
    // printf("\tVideo Memory: %x\n", VBE_INFO.video_memory);
    // printf("\tSoftware Rev: %x\n", VBE_INFO.software_rev);
    // printf("\tVendor: %s\n", VBE_INFO.vendor);
    // printf("\tProduct Name: %s\n", VBE_INFO.product_name);
    // printf("\tProduct Rev: %s\n", VBE_INFO.product_rev);
    // printf("\tOEM Strings: %s\n", VBE_INFO.oem_data);
    // printf("VESA MODES: \n");

    // uint16_t *modes = (long)VBE_INFO.video_modes;
    // while (1)
    // {
    //     printf(" %x --- ", *modes);
    //     modes++;
    //     if(*modes == 0xffff)
    //         break;
    // }

    // printf("VBE MODE INFO:\n");

    // printf("\tAttributes: %x\n", VBE_MODE_INFO.attributes);
    // printf("\tWidth: %d\n", VBE_MODE_INFO.width);
    // printf("\tHeight: %d\n", VBE_MODE_INFO.height);
    // printf("\tBPP: %d\n", VBE_MODE_INFO.bpp);
    // printf("\tFrame Buffer: %x\n", VBE_MODE_INFO.framebuffer);
    // printf("Frame Buffer: %x", VBE_MODE_INFO.framebuffer);

    // printf("\n");
    // modes++;
    // printf("Mode: %x --- %x\n", *modes, modes);
    // // while (*modes != 0xffff){
    // //     printf("Mode: %x --- %x\n", *modes, modes);
    // //     modes++;
    // // }

    framebuffer = (long)VBE_MODE_INFO.framebuffer;
    width = VBE_MODE_INFO.width;
    height = VBE_MODE_INFO.height;
    bpp = VBE_MODE_INFO.bpp;
    pitch = VBE_MODE_INFO.pitch;

    // uint32_t pixel_offset = y * pitch + (x * (bpp / 8)) + framebuffer;

    for (int i = 0; i < width; i+=2)
    {
        for(int j = 0; j < height; j+=2)
        {
            if(j % 100 > 50 || i % 100 > 50)
            {
                uint64_t* pixel_offset = (long)(j * pitch + (i * (bpp / 8)) + (uint32_t)framebuffer);
                *(pixel_offset) = 0x00ff000000ff0000;
            }
        }
    }
}