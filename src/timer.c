#include "timer.h"

void timer_tick(struct timer* t) {
    t->time++; 
}

void timer_reset(struct timer* t) {
    t->time = 0;
}