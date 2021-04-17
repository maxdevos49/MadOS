#ifndef __KERNEL_DEVICES_MOUSE_H
#define __KERNEL_DEVICES_MOUSE_H 1

#include <stdint.h>

#define MOUSE_COMMAND_PORT 0x60
#define MOUSE_DATA_PORT 0x64

#define MOUSE_ACKNOWLEDGE 0xfa //ACK

#define MOUSE_COM_ENABLE 0xf4
#define MOUSE_COM_DISABLE 0xf5
#define MOUSE_COM_RESET 0xff
#define MOUSE_COM_RESEND 0xfe

#define MOUSE_COM_SET_DEFAULT 0xf6
#define MOUSE_COM_SET_SAMPLE_RATE 0xf3
#define MOUSE_COM_SET_RESOLUTION 0xe8
#define MOUSE_COM_GET_MOUSEID 0xf2
#define MOUSE_COM_REQUEST_SINGLE_PACKET 0xeb
#define MOUSE_COM_STATUS 0xe9

struct mouse_packet
{
    uint8_t y_overflow : 1;
    uint8_t x_overflow : 1;
    uint8_t y_sign : 1;
    uint8_t x_sign : 1;
    uint8_t middle_button : 1;
    uint8_t right_button : 1;
    uint8_t left_button : 1;
    uint8_t x_movement;
    uint8_t y_movement;
    //Only set if the mouseID is 3 or 4
    uint8_t z_movement;
};

struct mouse_status
{
    uint8_t mode : 1;
    uint8_t enable : 1;
    uint8_t scaling : 1;
    uint8_t left_button : 1;
    uint8_t middle_button : 1;
    uint8_t right_button : 1;
    uint8_t resolution;
    uint8_t sample_rate;
};

void MOUSE_install();
void MOUSE_stream_enable();
void MOUSE_stream_disable();
void MOUSE_command(uint8_t command);
int MOUSE_get_id();
int MOUSE_poll(struct mouse_packet *data);
int MOUSE_status(struct mouse_status *status);

#endif