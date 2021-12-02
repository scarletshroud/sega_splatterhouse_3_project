#include "player.h"
#include "resources.h"

const float acceleration = 1.2f; 

void player_init(struct player *p, float pos_x, float pos_y) {
    p->health = DEFAULT_HEALTH;
    p->rage = DEFAULT_RAGE;  
    p->lifes = DEFAULT_LIFES;  
    p->dx = DEFAULT_DX; 
    p->dy = DEFAULT_DY;
    p->pos_x = pos_x; 
    p->pos_y = pos_y;
    p->sprite = SPR_addSprite(&rick, p->pos_x, p->pos_y, TILE_ATTR(PAL1, 0, FALSE, FALSE)); 
}


void update_player(struct player *const p) {   
    p->pos_x += p->dx;
    p->pos_y += p->dy;
    SPR_setPosition(p, p->pos_x, p->pos_y); 
    p->dx = 0;
    p->dy = 0; 
}

void player_move_left(struct player  *const p) {
    p->dx = -acceleration;  
}

void player_move_right(struct player *const p) {
    p->dx = acceleration; 
}

void player_move_up(struct player *const p) {
    p->dy = -acceleration; 
}

void player_move_down(struct player *const p) {
    p->dy = acceleration; 
}

void player_jump(struct player *const p) {
}