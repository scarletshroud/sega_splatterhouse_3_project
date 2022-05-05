#ifndef _PLAY_STATE_H_
#define _PLAY_STATE_H_

#include <genesis.h>

#include "zombie_list.h"

#define SCREEN_LEFT_SIDE -50
#define SCREEN_RIGHT_SIDE 250

#define FLOOR_UPPER_SIDE 80
#define FLOOR_DOWN_SIDE 120

//struct zombie_list* head;
struct zombie* zombies[3];

void play_state_init(); 
void play_state_render(); 
void play_state_update(); 
void play_state_clean(); 

#endif