#ifndef __KERNEL_TIME_RTC_H
#define __KERNEL_TIME_RTC_H 1

#include <stdint.h>
#include <stdbool.h>

#define CURRENT_YEAR 2020

#define RTC_HOUR_REGISTER   0x04
#define RTC_MINUTE_REGISTER 0x02
#define RTC_SECOND_REGISTER 0x00
#define RTC_MONTH_REGISTER  0x08
#define RTC_DAY_REGISTER    0x07
#define RTC_YEAR_REGISTER   0x09

#define RTC_STATUS_REGISTER_A 0x0A
#define RTC_STATUS_REGISTER_B 0x0B
#define RTC_STATUS_REGISTER_C 0x0C

#define RTC_IRQ8_ENABLE 0x40

struct RTC_time
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t century;
    uint16_t year;
};

bool RTC_get_update_in_progress_flag();
struct RTC_time RTC_read_time();

void RTC_enable_periodic_irq8();
void RTC_disable_periodic_irq8();
void RTC_set_periodic_rate(uint8_t rate);

void RTC_acknowledge_irq8();

#endif