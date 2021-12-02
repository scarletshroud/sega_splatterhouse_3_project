#ifndef _TITLE_STATE_H_
#define _TITLE_STATE_H_

#include <genesis.h>

static const short welcome_text_pos_x = 9; 
static const short welcome_text_pos_y = 30; 
static short offset_x;
static short offset_y; 

void title_state_init(); 
void tiltle_state_render(); 
void title_state_update(); 
void title_state_clean(); 

#endif