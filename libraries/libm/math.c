#include <math.h>

double pow(double x, double y)
{
    int ay = fabs(y);

    if (x == 0)
        return 0;

    if (ay == 0)
        return 1;

    double result = x;
    for (int i = 0; i < ay - 1; i++)
        result *= x;

    return y < 0 ? 1 / result : result;
}

double sqrt(double x)
{
    double res;
    asm volatile("fsqrt"
                 : "=t"(res)
                 : "0"(x));

    return res;
}

double ceil(double x)
{
    return __builtin_ceil(x);
}

double floor(double x)
{
    return __builtin_floor(x);
}

double fabs(double x)
{
    return __builtin_fabs(x);
}