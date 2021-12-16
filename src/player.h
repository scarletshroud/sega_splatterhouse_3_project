#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>

#include "zombie.h"

enum MOVE_DIRECTION {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_RIGHT,
    DIRECTION_LEFT,
    DIRECTION_NONE
};

enum MOVE_DIRECTION player_direction;

struct player_position {
    fix16 x; 
    fix16 y;
};

void player_init(const fix16 pos_x, const fix16 pos_y); 
void update_player();
void player_move(enum MOVE_DIRECTION direction);
void player_jump();
void player_hit(struct zombie *z);

struct player_position get_player_position(); 

#endif