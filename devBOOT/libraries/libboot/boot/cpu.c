#include "cpu.h"
#include "cpuid.h"

void CPU_info(struct CPU *info)
{
    printf("CPU information -> ");

    //Brand String
    int eax, ebx, ecx, edx;
    char *str = info->brand_string;

    __cpuid(0, eax, ebx, ecx, edx);

    ((int *)str)[0] = ebx;
    ((int *)str)[1] = edx;
    ((int *)str)[2] = ecx;
    str[12] = '\0';

    printf("Brand: %s, ", info->brand_string);

    //Model String
    str = info->model_string;

    __cpuid(0x80000002, eax, ebx, ecx, edx);

    ((int *)str)[0] = eax;
    ((int *)str)[1] = ebx;
    ((int *)str)[2] = ecx;
    ((int *)str)[3] = edx;

    __cpuid(0x80000003, eax, ebx, ecx, edx);

    ((int *)str)[4] = eax;
    ((int *)str)[5] = ebx;
    ((int *)str)[6] = ecx;
    ((int *)str)[7] = edx;

    __cpuid(0x80000004, eax, ebx, ecx, edx);

    ((int *)str)[8] = eax;
    ((int *)str)[9] = ebx;
    ((int *)str)[10] = ecx;
    ((int *)str)[11] = edx;
    str[49] = '\0';

    printf("Model: %s\n", info->model_string);
}