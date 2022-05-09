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
    ZOMBIE_STATE_INJURED_STAND,
    ZOMBIE_STATE_INJURED_WALK,
    ZOMBIE_STATE_INJURED_HIT,
    ZOMBIE_STATE_ATTACK,
    ZOMBIE_STATE_WALK_AWAY,
    ZOMBIE_STATE_DEAD
};

struct zombie {
    fix16 pos_x; 
    fix16 pos_y; 
    fix16 dx; 
    fix16 dy;
    fix16 velocity; 
    int8_t health;
    int8_t damage;
    uint8_t frame_counter;  
    enum ZOMBIE_STATE state;
    bool injured;
    Sprite *sprite;
    fix16 width; 
    fix16 height;
    int8_t target_distance;
};

struct zombie* create_zombie(const fix16 start_pos_x, const fix16 start_pos_y);
void zombie_update(struct zombie *z);
void bang_zombie(struct zombie *z, const int8_t damage);

#endif

