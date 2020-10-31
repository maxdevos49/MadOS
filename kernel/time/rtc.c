#include <kernel/time/rtc.h>
#include <kernel/time/cmos.h>
#include <kernel/interrupts/irq.h>
#include <stdio.h>

bool RTC_get_update_in_progress_flag()
{
    return (CMOS_read_register(RTC_STATUS_REGISTER_A) & 0x80);
}

struct RTC_time RTC_read_time()
{
    struct RTC_time time;
    struct RTC_time last_time;

    while (RTC_get_update_in_progress_flag())
    {
        time.month = CMOS_read_register(RTC_MONTH_REGISTER);
        time.day = CMOS_read_register(RTC_DAY_REGISTER);
        time.year = CMOS_read_register(RTC_YEAR_REGISTER);
        time.hour = CMOS_read_register(RTC_HOUR_REGISTER);
        time.minute = CMOS_read_register(RTC_MINUTE_REGISTER);
        time.second = CMOS_read_register(RTC_SECOND_REGISTER);

        if (CMOS_get_century_register() != 0)
            time.century = CMOS_read_register(CMOS_get_century_register());
    }

    do
    {
        last_time.month = time.month;
        last_time.day = time.day;
        last_time.year = time.year;
        last_time.hour = time.hour;
        last_time.minute = time.minute;
        last_time.second = time.second;
        last_time.century = time.century;

        time.month = CMOS_read_register(RTC_MONTH_REGISTER);
        time.day = CMOS_read_register(RTC_DAY_REGISTER);
        time.year = CMOS_read_register(RTC_YEAR_REGISTER);
        time.hour = CMOS_read_register(RTC_HOUR_REGISTER);
        time.minute = CMOS_read_register(RTC_MINUTE_REGISTER);
        time.second = CMOS_read_register(RTC_SECOND_REGISTER);

        if (CMOS_get_century_register() != 0)
            time.century = CMOS_read_register(CMOS_get_century_register());

    } while (last_time.month != time.month || last_time.day != time.day || last_time.year != time.year || last_time.hour != time.hour || last_time.minute != time.minute || last_time.second != time.second || last_time.century != time.century);

    uint8_t status_register_b = CMOS_read_register(RTC_STATUS_REGISTER_B);

    //if true its in bcd format so convert it to normal binary
    if (!(status_register_b & 0x04))
    {
        time.month = (time.month & 0x0f) + ((time.month / 16) * 10);
        time.day = (time.day & 0x0f) + ((time.day / 16) * 10);
        time.year = (time.year & 0x0f) + ((time.year / 16) * 10);
        time.hour = ((time.hour & 0x0f) + ((time.hour / 16) * 10)) | (time.hour & 0x80);
        time.minute = (time.minute & 0x0f) + ((time.minute / 16) * 10);
        time.second = (time.second & 0x0f) + ((time.second / 16) * 10);
        time.century = (time.century & 0x0f) + ((time.century / 16) * 10);
    }
    // if (!(status_register_b & 0x02) && (time.hour & 0x80))
    // {
    //     time.hour = ((time.hour & 0x7F) + 12) % 24;
    // }

    if (CMOS_get_century_register() != 0)
    {
        time.year += time.century * 100;
    }
    else
    {
        time.year += (CURRENT_YEAR / 100) * 100;
        if (time.year < CURRENT_YEAR)
            time.year += 100;
    }

    return time;
}

/**
 * Enables IRQ8 triggered by the RTC
 * */
void RTC_enable_periodic_irq8()
{
    uint64_t flags = IRQ_stash_and_disable();

    uint8_t previous_value = CMOS_read_register(RTC_STATUS_REGISTER_B);
    CMOS_write_register(RTC_STATUS_REGISTER_B, previous_value | RTC_IRQ8_ENABLE);

    IRQ_restore(flags);
}

/**
 * Disables IRQ8 triggered by the RTC
 * */
void RTC_disable_periodic_irq8()
{
    uint64_t flags = IRQ_stash_and_disable();

    RTC_set_periodic_rate(0);

    IRQ_restore(flags);
}

/**
 * Sets the periodic rate of IRQ8 triggered by the RTC
 * Valid rates are 3-15.
 * Rate - Frequency
 * 0 - Disable IRQ8
 * 1 - unstable
 * 2 - unstable 
 * 3 - 8192 hz
 * 4 - 4096 hz
 * 5 - 2048 hz
 * 6 - 1024 hz - Default
 * 7 - 512 hz
 * 8 - 256 hz
 * 9 - 128 hz
 * 10 - 64 hz
 * 11 - 32 hz
 * 12 - 16 hz
 * 13 - 8 hz
 * 14 - 4 hz
 * 15 - 2 hz
 * */
void RTC_set_periodic_rate(uint8_t rate)
{
    rate &= 0x0f; //Valid range is 2 - 15. This garuntees its atleast 0-15

    uint64_t flags = IRQ_stash_and_disable();

    uint8_t p = CMOS_read_register(RTC_STATUS_REGISTER_A);
    CMOS_write_register(RTC_STATUS_REGISTER_A, (p & 0xf0) | rate);

    IRQ_restore(flags);
}

/**
 * Acknowledges a interrupt from the RTC
 * */
void RTC_acknowledge_irq8()
{
    CMOS_read_register(RTC_STATUS_REGISTER_C);
}