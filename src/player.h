#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>

#include "list.h"
#include "zombie.h"
#include "position_util.h"

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
void player_update();
void player_jump();
void player_attack(struct zombie_list* list);
void player_set_direction(enum MOVE_DIRECTION direction);

struct player_position get_player_position(); 

#endif