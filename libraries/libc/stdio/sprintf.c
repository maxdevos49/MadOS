#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

enum FLAGS
{
    NONE,
    MINUS,
    PLUS,
    SPACE,
    POUND,
    ZERO
};
static int hex_to_str(char *buffer, uint64_t value, enum FLAGS flag, int width, int length);
static int int_to_str(char *buffer, int64_t, enum FLAGS flag, int width, int length);
static int float_to_str(char *buffer, double value, enum FLAGS flag, int width, int precision);

#define __PUTC(c, buffer, position) \
    {                               \
        buffer[position] = c;       \
        position++;                 \
    }

#define __PUTS(src, buffer, position)   \
    {                                   \
        strcpy(buffer + position, src); \
        position += strlen(src);        \
    }

/**
 * sprintf(buffer_string ,"format string", formatter_values...) 
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
 *  TODO not supported yet
 *  Special extra formatters:
 *      %(3)d  --> minimum width is 3 columns
 *      %(-)3d  --> right pad formatted value minimum width of 3
 *      %(0)3d  --> left pad zeros to reach minimum width
 *      %(+)3d  --> explicitly indicate the value is positive
 *      %(2.4)f  --> atleast 2 characters wide and display minimum of 4 decimal places
 * */
int sprintf(char *buffer, const char *restrict format, ...)
{
    va_list args;
    va_start(args, format);

    size_t written = vsprintf(buffer, format, args);

    va_end(args);
    return written;
}

static enum FLAGS is_flag(char c)
{
    switch (c)
    {
    case '-':
        return MINUS;
    case '+':
        return PLUS;
    case ' ':
        return SPACE;
    case '#':
        return POUND;
    case '0':
        return ZERO;
    }

    return NONE;
}

static int num_defined(char c)
{
    int n = ((int)c) - 48;
    if (n < 0 || n > 9)
        return 0;

    return n;
}

int vsprintf(char *buffer, const char *format, va_list args)
{
    size_t written = 0;
    const char *src = NULL;

    enum FLAGS flag;
    int width;
    int precision;
    int length;

    while (*format != '\0')
    {

        if (*format != '%')
        {
            __PUTC(*format, buffer, written);
            format++;
            continue;
        }
        else
        {

            flag = NONE;
            width = 0;
            precision = 0;
            length = 0;

            format++; //Move past %

            //Flags
            if ((flag = is_flag(*format)) != NONE)
                format++;

            //Width
            if ((width = num_defined(*format)) > 0)
                format++;

            //Precision
            if (*format == '.')
            {
                format++;

                if ((precision = num_defined(*format)) > 0)
                    format++;
            }

            //Length
            // if ((length = num_defined(*format)) > 0)
            //     format++;

            switch (*format)
            {
            case 'd': //%d --> decimal integer
            case 'i': //%i --> decimal integer

                written += int_to_str(buffer + written, (int64_t)va_arg(args, int64_t), flag, width, length);

                format++;
                break;
            case 'u': //%u --> unsigned integer
                written += int_to_str(buffer + written, (uint64_t)va_arg(args, uint64_t), flag, width, length);

                format++;
                break;
            case 'f': //%f --> float
                written += float_to_str(buffer + written, (double)va_arg(args, double), flag, width, precision);

                format++;
                break;
            case 'x': //%x --> unsigned hex
                written += hex_to_str(buffer + written, (uint64_t)va_arg(args, uint64_t), flag, width, length);

                format++;
                break;
            case 'c': //%c --> char
                      // char promotes to int
                __PUTC((char)va_arg(args, int), buffer, written);

                format++;
                break;
            case 's': //%s --> string

                src = (const char *)va_arg(args, const char *);
                __PUTS(src, buffer, written);

                format++;
                break;
            default: //%% or invalid specifier

                __PUTC(*format, buffer, written);

                format++;
                break;
            }
        }
    }

    __PUTC('\0', buffer, written);

    return written;
}

static int position_text(char *dst, const char *src, int src_len, enum FLAGS flag, int width, int length, int precision)
{

    int write_length;
    if (src_len < width + ((precision) ? precision + 1 : 0))
        write_length = width + ((precision) ? precision + 1 : 0);
    else
        write_length = src_len;

    int write_index = write_length - src_len;

    switch (flag)
    {
    // case
    case POUND:
    case NONE:
    case PLUS:
    case SPACE:
        memset(dst, ' ', write_length);
        break;
    case ZERO:
        memset(dst, '0', write_length);
        break;
    case MINUS:
        write_index = 0;
        break;
    }

    for (int i = 0; src[i] != '\0'; i++)
        dst[i + write_index] = src[i];

    return write_length;
}

static const char *HEX_TABLE = "0123456789abcdef"; //This is for simplicity but not really required
static int hex_to_str(char *buffer, uint64_t value, enum FLAGS flag, int width, int length)
{
    char hex_buffer[25];

    uint8_t hex_index = 0;
    uint8_t *ptr;

    for (int8_t i = 7; i > -1; i--)
    {
        ptr = ((uint8_t *)&value + i);

        if (*ptr == 0 && hex_index == 0)
        {
            continue;
        }
        else
        {
            hex_buffer[hex_index] = HEX_TABLE[((*ptr & 0xf0) >> 4)];
            hex_index++;

            hex_buffer[hex_index] = HEX_TABLE[(*ptr & 0x0f)];
            hex_index++;
        }
    }

    hex_buffer[hex_index] = 0;

    return position_text(buffer, hex_buffer, hex_index, flag, width, length, 0);
}

static int int_to_str(char *buffer, int64_t value, enum FLAGS flag, int width, int length)
{
    char int_buffer[25];

    uint8_t is_negative = 0;

    if (value < 0)
    {
        is_negative = 1;
        value *= -1;
        int_buffer[0] = '-';
    }

    uint8_t size = 0;
    uint64_t size_tester = (uint64_t)value;

    while (size_tester / 10 > 0)
    {
        size_tester /= 10;
        size++;
    }

    uint8_t index = 0;
    uint64_t new_value = (uint64_t)value;
    while (new_value / 10 > 0)
    {
        uint8_t remainder = new_value % 10;
        new_value /= 10;
        int_buffer[is_negative + size - index] = remainder + 48;
        index++;
    }

    uint8_t remainder = new_value % 10;
    int_buffer[is_negative + size - index] = remainder + 48;
    int_buffer[is_negative + size + 1] = 0;

    return position_text(buffer, int_buffer, index, flag, width, length, 0);
}

static int float_to_str(char *buffer, double value, enum FLAGS flag, int width, int precision)
{
    char float_buffer[25];

    int size = int_to_str(float_buffer, (int)value, 0, 0, 0);
    char *int_ptr = (char *)(float_buffer);
    char *float_ptr = float_buffer;

    if (value < 0)
        value *= -1;

    while (*int_ptr != 0)
    {
        *float_ptr = *int_ptr;
        int_ptr++;
        float_ptr++;
    }

    *float_ptr = '.';
    float_ptr++;

    float new_value = value - (int)value;

    for (uint8_t i = 0; i < precision; i++)
    {
        new_value *= 10;
        *float_ptr = (int)new_value + 48;
        new_value -= (int)new_value;
        float_ptr++;
    }

    *float_ptr = 0;

    return position_text(buffer, float_buffer, precision + size, flag, width, 0, precision);
}