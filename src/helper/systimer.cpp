#include "systimer.h"

SysTimer::SysTimer(long interval_ms)
{
    this->interval = interval_ms;

    starttime = get_boottime_in_ms();
}

SysTimer::~SysTimer()
{
}

void SysTimer::start()
{
    starttime = get_boottime_in_ms();
}

bool SysTimer::timeout()
{
    long long current = get_boottime_in_ms();

    if (current - starttime >= interval) {
        starttime = current;
        return true;
    } else {
        return false;
    }
}

long long SysTimer::get_boottime_in_ms()
{
    long long time;
    struct timespec times = {0, 0};

    clock_gettime(CLOCK_BOOTTIME, &times);

    time = times.tv_sec;
    time = time * 1000 + times.tv_nsec / 1000000;

    return  time;
}
