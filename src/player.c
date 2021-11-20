#include "headers/player.h"
#include "resources.h"

void playerInit(struct player* p, float pos_x, float pos_y) 
{
    p->health = DEFAULT_HEALTH;
    p->rage = DEFAULT_RAGE;  
    p->lifes = DEFAULT_LIFES;  
    p->dx = DEFAULT_DX; 
    p->dy = DEFAULT_DY;
    p->pos_x = pos_x; 
    p->pos_y = pos_y;
    p = SPR_addSprite(&rick, p->pos_x, p->pos_y, TILE_ATTR(PAL1, 0, FALSE, FALSE)); 
}

void playerMove(struct player* p) 
{   
    p->pos_x += p->dx;
    p->pos_y += p->dy;
    SPR_setPosition(p, p->dx, p->dy); 
    p->dx = 0;
    p->dy = 0; 
}

void playerJump(struct player* p) 
{

}