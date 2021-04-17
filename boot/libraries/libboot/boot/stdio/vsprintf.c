#include "../stdio.h"
#include "../string.h"
#include "../stdarg.h"

enum FLAGS
{
    NONE,
    MINUS,
    PLUS,
    SPACE,
    POUND,
    ZERO
};

static int hex_to_str(char *buffer, uint64_t value, enum FLAGS flag, int width);
static int int_to_str(char *buffer, int64_t, enum FLAGS flag, int width);
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
    uint64_t written = 0;
    const char *src = NULL;

    enum FLAGS flag;
    int width;
    int precision;

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

                written += int_to_str(buffer + written, (int64_t)va_arg(args, int64_t), flag, width);

                format++;
                break;
            case 'u': //%u --> unsigned integer
                written += int_to_str(buffer + written, (uint64_t)va_arg(args, uint64_t), flag, width);

                format++;
                break;
            case 'f': //%f --> float
                written += float_to_str(buffer + written, (double)va_arg(args, double), flag, width, precision);

                format++;
                break;
            case 'x': //%x --> unsigned hex
                written += hex_to_str(buffer + written, (uint64_t)va_arg(args, uint64_t), flag, width);

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

static int position_text(char *dst, const char *src, int src_len, enum FLAGS flag, int width, int precision)
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
static int hex_to_str(char *buffer, uint64_t value, enum FLAGS flag, int width)
{
    char hex_buffer[25];

    uint8_t hex_index = 0;
    uint8_t *ptr;

    for (char i = 7; i > -1; i--)
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

    return position_text(buffer, hex_buffer, hex_index, flag, width, 0);
}

static int int_to_str(char *buffer, int64_t value, enum FLAGS flag, int width)
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

    while ((size_tester / 10) > 0)
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
    index++;

    return position_text(buffer, int_buffer, index, flag, width, 0);
}

static int float_to_str(char *buffer, double value, enum FLAGS flag, int width, int precision)
{
    char float_buffer[30];

    int int_num = (int)value;
    int int_size = int_to_str(float_buffer, int_num, 0, 0); //Get rounded int value

    char *di = float_buffer + int_size + 1;                 // String Destination index

    *(di-1) = '.';                                          // Place Decimal point

    if (value < 0)                                          // Float math fails with negative numbers
        value = -value;                                     // Flip the sign

    float decimal_portion = value - int_num;                // Get only the decimal portion

    uint8_t i;
    for (i = 0; i < precision; i++)
    {
        decimal_portion *= 10;                              // Move the decimal place one to the right
        *di = (int)decimal_portion + 48;                    // The new integer portion +48 equals the ascii character
        decimal_portion -= (int)decimal_portion;            // Get only the remaining decimal portion again
        di++;                                               // Increase the destination pointer
    }

    *di = '\0';                                             // Terminate the string

    return position_text(buffer, float_buffer, int_size + 1 + precision, flag, width, precision);
}