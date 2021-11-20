#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>

#define DEFAULT_HEALTH 100
#define DEFAULT_RAGE 0
#define DEFAULT_LIFES 3 
#define DEFAULT_DX 0 
#define DEFAULT_DY 0

struct player 
{
    u16 health;
    u16 rage;
    u16 lifes;
    short dx; 
    short dy; 
    float pos_x; 
    float pos_y;
    Sprite* sprite; 
}; 

void playerInit(struct player* p, float posX, float posY); 
void playerMove(struct player* p);
void playerJump(); 

#endif