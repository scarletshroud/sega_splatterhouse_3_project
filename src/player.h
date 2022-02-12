#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>

enum PLAYER_STATE {
    STATE_STAND,
    STATE_WALK,
    STATE_JUMP,
    STATE_HIT,
    STATE_JUMP_HIT  
}; 

enum PLAYER_MOVE_DIRECTION {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_RIGHT,
    DIRECTION_LEFT,
    DIRECTION_NONE
};

struct player_position {
    fix16 x; 
    fix16 y;
};

void player_init(const fix16 pos_x, const fix16 pos_y); 
void player_update();
void player_set_direction(enum PLAYER_MOVE_DIRECTION direction);
void player_set_state(enum PLAYER_STATE state);
enum PLAYER_MOVE_DIRECTION player_get_direction();

struct player_position get_player_position(); 

#endif