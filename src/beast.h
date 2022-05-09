#ifndef _BEAST_H_
#define _BEAST_H_

#include <genesis.h>
#include "position.h"

enum BEAST_STATE {
    BEAST_STATE_WAIT,
    BEAST_STATE_STAND,
    BEAST_STATE_WALK, 
    BEAST_STATE_HIT, 
    BEAST_STATE_BANG, 
    BEAST_STATE_JUMP,
    BEAST_STATE_LOSING_HEAD,
    BEAST_STATE_INJURED_STAND,
    BEAST_STATE_INJURED_WALK,
    BEAST_STATE_INJURED_HIT,
    BEAST_STATE_INJURED_BANG,
    BEAST_STATE_INJURED_JUMP,
    BEAST_STATE_INJURED_SUPER_HIT,
    BEAST_STATE_DIE,
    BEAST_STATE_ATTACK,
    BEAST_STATE_WALK_AWAY,
    BEAST_STATE_DEAD
};

struct beast {
    fix16 pos_x; 
    fix16 pos_y; 
    fix16 dx; 
    fix16 dy;
    fix16 velocity; 
    int16_t health;
    int8_t damage;
    uint8_t frame_counter;  
    enum BEAST_STATE state;
    bool injured;
    Sprite *sprite;
    fix16 width; 
    fix16 height;
    int8_t target_distance;
};

void beast_init(const fix16 start_pos_x, const fix16 start_pos_y);
void beast_update();
void bang_beast(const int8_t damage);

struct position beast_get_position(); 

fix16 beast_height();
fix16 beast_width();

#endif