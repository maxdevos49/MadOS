#ifndef __BOOT_DRIVERS_SERIAL
#define __BOOT_DRIVERS_SERIAL 1

void SERIAL_init();
char SERIAL_read();
void SERIAL_write(char byte);

#endif