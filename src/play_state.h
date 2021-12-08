#ifndef _PLAY_STATE_H_
#define _PLAY_STATE_H_

#include <genesis.h>

#include "zombie.h"

struct zombie z;

void play_state_init(); 
void play_state_render(); 
void play_state_update(); 
void play_state_clean(); 

#endif