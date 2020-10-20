#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static char *hex_to_str(uint64_t);
static char *int_to_str(int64_t);
static char *float_to_str(double, uint8_t);

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

                written += puts(int_to_str((int64_t)va_arg(args, int64_t)));

                format += 2;
                break;
            case 'u': //%u --> unsigned integer

                written += puts(int_to_str((uint64_t)va_arg(args, uint64_t)));

                format += 2;
                break;
            case 'f': //%f --> float

                written += puts(float_to_str((double)va_arg(args, double), 3));

                format += 2;
                break;
            case 'x': //%x --> unsigned hex

                written += puts(hex_to_str((uint64_t)va_arg(args, uint64_t)));

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

static char hex_string_output[17]; // 8bytes == 16 characters  + null byte = 17 total characters
static const char *HEX_TABLE = "0123456789abcdef"; //This is for simplicity but not really required
static char *hex_to_str(uint64_t value)
{

    if (value == 0)
    {
        hex_string_output[0] = '0';
        hex_string_output[1] = '\0';
        return hex_string_output;
    }

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
            hex_string_output[hex_index] = HEX_TABLE[((*ptr & 0xf0) >> 4)];
            hex_index++;

            hex_string_output[hex_index] = HEX_TABLE[(*ptr & 0x0f)];
            hex_index++;
        }
    }

    hex_string_output[hex_index] = 0;

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
static char *float_to_str(double value, uint8_t decimal_places)
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

#include <ctest.h>

void test_printf(void)
{

    printf("\t\tstatic char* int_to_str(int64_t)\n");

    ctest_assert("Error: string not \"0\"", strcmp(int_to_str(0), "0") == 0);
    ctest_assert("Error: string not \"100\"", strcmp(int_to_str(100), "100") == 0);
    ctest_assert("Error: string not \"-100\"", strcmp(int_to_str(-100), "-100") == 0);
    ctest_assert("Error: string not \"2000000000\"", strcmp(int_to_str(2000000000), "2000000000") == 0);

    printf("\t\tstatic char* hex_to_str(uint64_t)\n");

    ctest_assert("Error: \"0\" was not outputted", strcmp("0", hex_to_str(0x00)) == 0);
    ctest_assert("Error: \"face\" was not outputted", strcmp("face", hex_to_str(0xface)) == 0);
    ctest_assert("Error: \"1234\" was not outputted", strcmp("1234", hex_to_str(0x1234)) == 0);
    ctest_assert("Error: \"1234abcdface\" was not outputted", strcmp("1234abcdface", hex_to_str(0x1234abcdface)) == 0);

    printf("\t\tstatic char* hex_to_str(uint64_t)\n");

    ctest_assert("Error: \"0.00\" was not outputted", strcmp("0.000", float_to_str(0, 3)) == 0);
    ctest_assert("Error: \"1.000\" was not outputted", strcmp("1.000", float_to_str(1, 3)) == 0);
    ctest_assert("Error: -456.12 should output \"-456.119\"", strcmp("-456.119", float_to_str(-456.12, 3)) == 0);
    
}

#endif