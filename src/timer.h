#ifndef _TIMER_H_
#define _TIMER_H_

#include <genesis.h>

struct timer {
    uint16_t time;
};

uint16_t timer_time(struct timer* t);
void timer_tick(struct timer* t);
void timer_reset(struct timer* t);

#endif