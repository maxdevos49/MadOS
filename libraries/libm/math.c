#include <math.h>

double pow(double x, double y)
{
    if (y == 0)
    {
        return 1;
    }
    else if (y == 1)
    {
        return x;
    }
  

    double result = x;
    for (int i = 0; i < y-1; i++)
        result *= x;

    return result;
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