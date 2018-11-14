#ifndef SYSTIMER_H
#define SYSTIMER_H
#include <stdio.h>
#include <time.h>

enum TimerType {
    SINGLE,
    CYCLE
};

class SysTimer
{
public:
    long long starttime;
    long interval;

public:
    SysTimer(long interval_ms);
    ~SysTimer();

    void start();
    bool timeout();

private:
    long long get_boottime_in_ms();
};

#endif // SYSTIMER_H
