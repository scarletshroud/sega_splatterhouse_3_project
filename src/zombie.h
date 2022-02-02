#ifndef _ZOMBIE_H_
#define _ZOMBIE_H_

#include <genesis.h>
#include "player.h"
#include "position_util.h"

enum ZOMBIE_STATE {
    ZOMBIE_STATE_STAND,
    ZOMBIE_STATE_WALK, 
    ZOMBIE_STATE_HIT, 
    ZOMBIE_STATE_BANG, 
    ZOMBIE_STATE_DIE,
    ZOMBIE_STATE_NONE
};

struct zombie {
    fix16 pos_x; 
    fix16 pos_y; 
    fix16 dx; 
    fix16 dy;
    fix16 velocity; 
    int8_t health;
    uint8_t frame_counter;  
    enum ZOMBIE_STATE state;
    Sprite *sprite;
    fix16 width; 
    fix16 height; 
};

struct zombie* create_zombie(const fix16 start_pos_x, const fix16 start_pos_y);
void zombie_update(struct zombie *z);
void bang_zombie(struct zombie *z, const uint8_t damage);

#endif

