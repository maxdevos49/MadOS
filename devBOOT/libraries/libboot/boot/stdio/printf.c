
#include "../stdio.h"
#include "../stdint.h"

#include "../drivers/tty.h"
#include "../drivers/serial.h"

/**
 * printf(buffer_string ,"format string", formatter_values...) 
 * 
 * %[flags][width][.precision][length]specifier 
 * 
 * Format characters:
 *      %c --> character
 *      %s --> string
 *      %u --> unsigned integer
 *      %x --> hexadecimal
 *      %d, %i --> decimal integer
 *      %f --> float
 * 
 *  Special extra formatters:
 *      %(3)d  --> minimum width is 3 columns
 *      %(-)3d  --> right pad formatted value minimum width of 3
 *      %(0)3d  --> left pad zeros to reach minimum width
 *      %(+)3d  --> explicitly indicate the value is positive
 *      %(2.4)f  --> atleast 2 characters wide and display minimum of 4 decimal places
 * */
int printf(const char *restrict format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[400];

    uint64_t written = vsprintf(buffer, format, args);

    TTY_write(buffer, written);

    for (uint64_t i = 0; i < written; i++)
        SERIAL_write(buffer[i]);

    va_end(args);
    return written;
}
