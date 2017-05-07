#include "dusk/timer.h"

void dusk_timer_start(dusk_timer_t * this)
{
    this->_started = true;
#ifdef WIN32
    this->_point = clock();
#else
    clock_getres(CLOCK_MONOTONIC, &this->_res);
    clock_gettime(CLOCK_MONOTONIC, &this->_point);
#endif
}

double dusk_timer_get_ms(dusk_timer_t * this)
{
    if (!this->_started)
    {
        return 0;
    }

#ifdef WIN32
    return ((clock() - this->_point) * 1000.0) / CLOCKS_PER_SEC;
#else
    struct timespec now;
    struct timespec diff;

    clock_gettime(CLOCK_MONOTONIC, &now);

    if ((now.tv_nsec - this->_point.tv_nsec) < 0)
    {
        diff.tv_sec  = now.tv_sec - this->_point.tv_sec - 1;
        diff.tv_nsec = now.tv_nsec - this->_point.tv_nsec + 1000000000;
    }
    else
    {
        diff.tv_sec  = now.tv_sec - this->_point.tv_sec;
        diff.tv_nsec = now.tv_nsec - this->_point.tv_nsec;
    }
    return (diff.tv_sec * 1000.0) + (diff.tv_nsec / 1000000.0);
#endif
}
