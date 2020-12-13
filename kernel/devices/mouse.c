#include <kernel/devices/mouse.h>
#include <kernel/io.h>

#include <stdio.h>

static uint8_t mouse_id;

static inline void MOUSE_pre_read_wait()
{
    while ((inb(0x64) & 0x01) == 0)
        ;
}

static inline void MOUSE_pre_command_wait()
{
    //Wait until bit 2 is 1
    while ((inb(0x64) & 0x02) == 0x02)
        ;
}

static inline void MOUSE_read_packet(struct mouse_packet *data)
{
    uint8_t data_byte = inb(0x60);
    data->y_overflow = (data_byte & 0x80) >> 7;
    data->x_overflow = (data_byte & 0x40) >> 6;
    data->y_sign = (data_byte & 0x20) >> 5;
    data->x_sign = (data_byte & 0x10) >> 4;
    data->middle_button = (data_byte & 0x04) >> 2;
    data->right_button = (data_byte & 0x02) >> 1;
    data->left_button = (data_byte & 0x01);

    data->x_movement = inb(0x60);

    // if (data->x_sign)
    //     data->x_movement |= 0xff00;

    data->y_movement = inb(0x60);

    // if (data->y_sign)
    //     data->y_movement |= 0xff00;

    if (mouse_id > 2)
        data->z_movement = inb(0x60);
}

void MOUSE_command(uint8_t command)
{
    // Mouse + Keyboard share same port
    // so must wait for a status bit
    MOUSE_pre_command_wait();

    //Indicates next command is for mouse.
    //Default/without is for keyboard
    outb(0x64, 0xd4);

    //Send actual command
    outb(0x60, command);
}

void MOUSE_stream_enable()
{
    MOUSE_command(0xf4); //Enable packet streaming
}

void MOUSE_stream_disable()
{
    MOUSE_command(0xf5);
}

int MOUSE_get_id()
{
    MOUSE_command(0xf2);
    MOUSE_pre_read_wait();
    inb(0x60);//ack
    return inb(0x60);
}

int MOUSE_status(struct mouse_status *status)
{
    MOUSE_command(0xe9); //request status
    MOUSE_pre_read_wait();
    if (inb(0x60) == 0xfa)
    {
        uint8_t data_byte = inb(0x60);
        status->mode = (data_byte & 0x40) >> 6;
        status->enable = (data_byte & 0x20) >> 5;
        status->scaling = (data_byte & 0x10) >> 4;
        status->left_button = (data_byte & 0x04) >> 2;
        status->middle_button = (data_byte & 0x02) >> 1;
        status->right_button = (data_byte & 0x01);

        status->resolution = inb(0x60);
        status->sample_rate = inb(0x60);

        return 1;
    }

    return 0;
}

int MOUSE_poll(struct mouse_packet *data)
{
    if (inb(0x64) & 0x20)
    {
        MOUSE_read_packet(data);
        return 1;
    }

    return 0;
}

void MOUSE_install()
{
    printf("Installing Mouse\n");
    mouse_id = MOUSE_get_id();

    printf("Mouse id: %x\n", mouse_id);

    MOUSE_stream_enable();
}