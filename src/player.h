#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>

#define DEFAULT_DX 0 
#define DEFAULT_DY 0
#define DEFAULT_RAGE 0
#define DEFAULT_LIFES 3 
#define DEFAULT_HEALTH 100

struct player 
{
    uint8_t health;
    uint8_t rage;
    uint8_t lifes;
    float dx; 
    float dy; 
    float pos_x; 
    float pos_y;
    Sprite *sprite; 
}; 

void player_init(struct player *p, float pos_x, float pos_y); 
void update_player(struct player *const p);
void player_move_left(struct player *const p);
void player_move_right(struct player *const p);
void player_move_up(struct player *const p);
void player_move_down(struct player *const p); 
void player_jump(); 

#endif