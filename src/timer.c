#include "timer.h"

uint16_t timer_time(struct timer* t) {
    return t->time;
}

void timer_tick(struct timer* t) {
    t->time++; 
}

void timer_reset(struct timer* t) {
    t->time = 0;
}