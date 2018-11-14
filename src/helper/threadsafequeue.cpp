#include "threadsafequeue.h"

long long get_runtime_in_msecs()
{
    long long time;
    struct timespec times = {0, 0};

    clock_gettime(CLOCK_BOOTTIME, &times);

    time = times.tv_sec;
    time = time * 1000 + times.tv_nsec / 1000000;

    return  time;
}
