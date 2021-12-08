#include "zombie.h"

#include "resources.h"

#define ZOMBIE_SPRITE_WIDTH 32
#define ZOMBIE_SPRITE_HEIGHT 64   
#define ZOMBIE_DEFAULT_HEALTH 100

enum ZOMBIE_ANIM {
    ANIM_STAND,
    ANIM_WALK,
    ANIM_HIT,
    ANIM_BANG,
    ANIM_DIE,
};

void zombie_animate(struct zombie *z);

void zombie_init(struct zombie *z, fix16 start_pos_x, fix16 start_pos_y) {
    z->pos_x = start_pos_x; 
    z->pos_y = start_pos_y;

    z->health = ZOMBIE_DEFAULT_HEALTH; 
    z->state = ZOMBIE_STATE_STAND;

    z->sprite = SPR_addSprite(&zombie_sprite, fix16ToInt(z->pos_x), fix16ToInt(z->pos_y), TILE_ATTR(PAL2, 0, FALSE, FALSE));

    z->width = ZOMBIE_SPRITE_WIDTH;
    z->height = ZOMBIE_SPRITE_HEIGHT;

    z->frame_counter = 0;  
}

void zombie_update(struct zombie *z) {
    if (z->state == ZOMBIE_STATE_BANG || z->state == ZOMBIE_STATE_DIE) {
        z->frame_counter++;
    } 

    if (z->frame_counter == 10 && z->state == ZOMBIE_STATE_BANG) {
        z->state = ZOMBIE_STATE_STAND; 
        z->frame_counter = 0;
    }

    if (z->frame_counter == 30 && z->state == ZOMBIE_STATE_DIE) {
        SPR_setAnimAndFrame(z->sprite, 8, 6);
        z->state = ZOMBIE_STATE_NONE;
    }

    zombie_animate(z);
    SPR_setPosition(z->sprite, fix16ToInt(z->pos_x), fix16ToInt(z->pos_y));
}

void zombie_animate(struct zombie *z) {
    switch (z->state) {
        case ZOMBIE_STATE_STAND:
            SPR_setAnim(z->sprite, ANIM_STAND);
            break; 
        case ZOMBIE_STATE_WALK:
            SPR_setAnim(z->sprite, ANIM_WALK);
            break;
        case ZOMBIE_STATE_HIT:
            SPR_setAnim(z->sprite, ANIM_HIT);
            break; 
        case ZOMBIE_STATE_BANG:
            SPR_setAnim(z->sprite, ANIM_BANG);
            break; 
        case ZOMBIE_STATE_DIE:
            SPR_setAnim(z->sprite, ANIM_DIE);
            break; 
    }
}

void bang_zombie(struct zombie *z, uint8_t damage) {
    z->health = z->health - damage;
    if (z->health <= 0) {
        z->state = ZOMBIE_STATE_DIE; 
    } else {
        z->state = ZOMBIE_STATE_BANG;
    }
}