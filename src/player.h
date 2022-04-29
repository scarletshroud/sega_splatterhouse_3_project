#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <genesis.h>

enum PLAYER_STATE {
    STATE_STAND,
    STATE_WALK,
    STATE_JUMP,
    STATE_FIRST_HIT,
    STATE_SECOND_HIT,
    STATE_THIRD_HIT,
    STATE_JUMP_HIT,
    STATE_BEAST_TRANSORMATION,
    STATE_BEAST_STAND,
    STATE_BEAST_WALK,
    STATE_BEAST_JUMP,
    STATE_BEAST_FIRST_HIT,
    STATE_BEAST_SECOND_HIT,
    STATE_BEAST_THIRD_HIT,
    STATE_BEAST_JUMP_HIT,
    STATE_NORMAL_TRANSORMATION,
    STATE_DIE,
    STATE_DEAD  
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

void player_set_state(enum PLAYER_STATE state);
void player_set_direction(enum PLAYER_MOVE_DIRECTION direction);

enum PLAYER_STATE player_get_state();
enum PLAYER_MOVE_DIRECTION player_get_direction();
struct Sprite* player_get_sprite();

int16_t player_get_health(); 
uint16_t player_get_energy(); 
uint16_t player_get_lifes(); 

struct player_position player_get_position(); 

#endif