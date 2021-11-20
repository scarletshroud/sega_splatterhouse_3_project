#ifndef _PLAY_STATE_H_
#define _PLAY_STATE_H_

#include <genesis.h>
#include "player.h"

struct player* p;

void playStateInit(); 
void playStateRender(); 
void playStateUpdate(); 
void playStateClean(); 

#endif