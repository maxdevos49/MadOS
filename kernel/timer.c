#include <kernel/timer.h>
#include <kernel/interrupts/irq.h>
#include <kernel/time/pit.h>

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static volatile uint64_t pit_ticks = 0;

static volatile int64_t epoch_time = 0;
static volatile int64_t rtc_ticks = 0;

static void pit_handler()
{
    pit_ticks++;
}

static void rtc_handler()
{
    rtc_ticks++;

    if (rtc_ticks % 64 == 0){
        epoch_time++;
    }

    RTC_acknowledge_irq8();
}

void TIMER_install()
{
    //Install handlers
    IRQ_install_handler(0, pit_handler);
    IRQ_install_handler(8, rtc_handler);

    // //Configure PIT
    // PIT_set_periodic_frequency(TIMER_PIT_FREQUENCY);
    // PIT_enable_periodic_irq0();

    //Configure RTC
    RTC_set_periodic_rate(TIMER_RTC_RATE);
    RTC_enable_periodic_irq8();

    struct RTC_time t = RTC_read_time();
    TIMER_set_time(&t);
}

void TIMER_set_time(struct RTC_time *t)
{
    struct tm tm;

    tm.tm_sec = t->second;
    tm.tm_min = t->minute;
    tm.tm_hour = t->hour;
    tm.tm_mday = t->day;
    tm.tm_mon = t->month;
    tm.tm_year = t->year - 1900;

    epoch_time = mktime(&tm);
    printf("Time: %d\n", epoch_time);
}

int64_t TIMER_get_time()
{
    return epoch_time;
}

// void sleep_ticks(uint64_t ticks)
// {
//     uint64_t end_ticks = ticks + pit_ticks;

//     while (pit_ticks < end_ticks)
//         ;
// }
