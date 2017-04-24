#ifndef DUSK_TIMER_H
#define DUSK_TIMER_H

#include <dusk/config.h>
#include <stdbool.h>
#include <time.h>

typedef struct dusk_timer
{
  bool _started;
#ifdef WIN32
  clock_t _point;
#else
  struct timespec _res;
  struct timespec _point;
#endif
} dusk_timer_t;

void   dusk_timer_start(dusk_timer_t * this);
double dusk_timer_get_ms(dusk_timer_t * this);

#endif // DUSK_TIMER_H
