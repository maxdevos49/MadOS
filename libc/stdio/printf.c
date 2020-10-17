#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static char *hex_to_str(uint64_t, size_t);
static char *int_to_str(int64_t);
static char *float_to_str(float, uint8_t);

/**
 * printf("format string", formatter_values...) 
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
int printf(const char *restrict format, ...)
{
    va_list args;
    va_start(args, format);

    uint64_t written = 0;

    while (*format)
    {

        if (*format != '%')
        {
            putchar(*format);
            format++;
            written++;
            continue;
        }
        else
        {
            switch (format[1])
            {
            case 'd': //%d --> decimal integer
            case 'i': //%i --> decimal integer

                written += puts(int_to_str((int)va_arg(args, int)));

                format += 2;
                break;
            case 'u': //%u --> unsigned integer

                written += puts(int_to_str((unsigned int)va_arg(args, unsigned int)));

                format += 2;
                break;
            case 'f': //%f --> float

                written += puts(float_to_str((double)va_arg(args, double), 3));

                format += 2;
                break;
            case 'x': //%x --> unsigned hex

                written += puts(hex_to_str((int)va_arg(args, uint64_t), sizeof(int)));

                format += 2;
                break;
            case 'c': //%c --> char
                      // char promotes to int
                putchar((char)va_arg(args, int));
                written++;

                format += 2;
                break;
            case 's': //%s --> string

                written += puts((const char *)va_arg(args, const char *));

                format += 2;
                break;
            default: //%% or NULL
                putchar(*format);
                format += 2;
                written += 2;
                break;
            }
        }
    }

    va_end(args);
    return written;
}

static char hex_string_output[120];
static char *hex_to_str(uint64_t value, size_t size)
{
    size = size * 2;
    uint8_t *ptr;
    uint8_t temp;
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)&value + i); //select a byte of the number

        temp = ((*ptr & 0xf0) >> 4);                                           //grab left first 4 bits
        hex_string_output[(size - (i * 2 + 1))] = temp + (temp > 9 ? 55 : 48); //select letter

        temp = ((*ptr & 0x0f));
        hex_string_output[(size - (i * 2 + 0))] = temp + (temp > 9 ? 55 : 48);
    }

    //null terminate string
    hex_string_output[size + 1] = 0;

    return hex_string_output;
}

static char integer_string_output[60];
static char *int_to_str(int64_t value)
{
    uint8_t is_negative = 0;

    if (value < 0)
    {
        is_negative = 1;
        value *= -1;
        integer_string_output[0] = '-';
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
        integer_string_output[is_negative + size - index] = remainder + 48;
        index++;
    }

    uint8_t remainder = new_value % 10;
    integer_string_output[is_negative + size - index] = remainder + 48;
    integer_string_output[is_negative + size + 1] = 0;

    return integer_string_output;
}

static char float_string_output[60];
static char *float_to_str(float value, uint8_t decimal_places)
{
    char *int_ptr = (char *)int_to_str((int)value);
    char *float_ptr = float_string_output;

    if (value < 0)
    {
        value *= -1;
    }

    while (*int_ptr != 0)
    {
        *float_ptr = *int_ptr;
        int_ptr++;
        float_ptr++;
    }

    *float_ptr = '.';
    float_ptr++;

    float new_value = value - (int)value;

    for (uint8_t i = 0; i < decimal_places; i++)
    {
        new_value *= 10;
        *float_ptr = (int)new_value + 48;
        new_value -= (int)new_value;
        float_ptr++;
    }

    *float_ptr = 0;

    return float_string_output;
}

#ifdef __test

#include <minunit.h>

void test_printf(void)
{

    printf("\tstatic char* int_to_str(int64_t value)\n");

    int length = strlen(int_to_str(0));
    mu_assert("Error: length is not 1", length == 1);
    length = strlen(int_to_str(100));
    mu_assert("Error: length is not 3", length == 3);
    length = strlen(int_to_str(-100));
    mu_assert("Error: length is not 4", length == 4);

    // // mu_assert("Error test: Error test: Error test: Error test: Error test: Error test: Error test: Error test: Error test: Error test: Error test: Error test: Error test: Error test: ", 10 == 3);
    // int test = strcmp("face", hex_to_str(0xface, sizeof(char)));
    // printf("%d\n", test);
    // // mu_assert("Error: hex_to_str() provides incorrect output.",  test == 0);

    return;
}

#endif