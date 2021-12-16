#include "zombie.h"
#include "resources.h"

#define ZOMBIE_VELOCITY 2

#define ZOMBIE_DEFAULT_DX 0 
#define ZOMBIE_DEFAULT_DY 0 

#define ZOMBIE_SPRITE_WIDTH 32
#define ZOMBIE_SPRITE_HEIGHT 64   

#define ZOMBIE_DEFAULT_HEALTH 100

enum ZOMBIE_ANIM {
    ANIM_STAND,
    ANIM_WALK,
    ANIM_HIT,
    ANIM_BANG,
    ANIM_DIE
};

static void zombie_hit(struct zombie *z); 
static void zombie_walk(struct zombie *z);
static void zombie_attack(struct zombie *z); 
static void zombie_animate(struct zombie *z);

void zombie_init(struct zombie *z, fix16 start_pos_x, fix16 start_pos_y) {
    z->pos_x = start_pos_x; 
    z->pos_y = start_pos_y;

    z->dx = FIX16(ZOMBIE_DEFAULT_DX); 
    z->dy = FIX16(ZOMBIE_DEFAULT_DY);
    z->velocity = FIX16(ZOMBIE_VELOCITY); 

    z->health = ZOMBIE_DEFAULT_HEALTH; 
    z->state = ZOMBIE_STATE_STAND;

    z->sprite = SPR_addSprite(&zombie_sprite, fix16ToInt(z->pos_x), fix16ToInt(z->pos_y), TILE_ATTR(PAL2, 0, FALSE, FALSE));

    z->width = ZOMBIE_SPRITE_WIDTH;
    z->height = ZOMBIE_SPRITE_HEIGHT;

    z->frame_counter = 0;  
}

void zombie_update(struct zombie *z) {

    zombie_walk(z);
    //zombie_hit(z);

    if (z->state == ZOMBIE_STATE_BANG || z->state == ZOMBIE_STATE_DIE || z->state == ZOMBIE_STATE_HIT) {
        z->frame_counter++;
    } 

    if (z->frame_counter == 10 && z->state == ZOMBIE_STATE_BANG) {
        z->state = ZOMBIE_STATE_STAND; 
        z->frame_counter = 0;
    }

    if (z->frame_counter == 15 && z->state == ZOMBIE_STATE_HIT) {
        z->state = ZOMBIE_STATE_STAND;
        z->frame_counter = 0;
    } 

    if (z->frame_counter == 30 && z->state == ZOMBIE_STATE_DIE) {
        SPR_setAnimAndFrame(z->sprite, 8, 6);
        z->state = ZOMBIE_STATE_NONE;
    }

    z->pos_x = fix16Add(z->pos_x, z->dx);
    z->pos_y = fix16Add(z->pos_y, z->dy);

    zombie_animate(z);
    SPR_setPosition(z->sprite, fix16ToInt(z->pos_x), fix16ToInt(z->pos_y));
}

void zombie_hit(struct zombie *z) {

    const uint8_t impact_dist = 60; 

    if (z->state != ZOMBIE_STATE_STAND && z->state != ZOMBIE_STATE_WALK) {
        return; 
    }

    const struct player_position player_pos = get_player_position();

    if (abs(fix16ToInt(z->pos_x) - fix16ToInt(player_pos.x)) <= impact_dist) {
        z->state = ZOMBIE_STATE_HIT; 
    }
}

void zombie_walk(struct zombie *z) {
    const struct player_position player_pos = get_player_position(); 

    const int8_t distance_x = fix16ToInt(z->pos_x) - fix16ToInt(player_pos.x);
    const int8_t distance_y = fix16ToInt(z->pos_y) - fix16ToInt(player_pos.y); 

    if (abs(distance_x) > 35) {
        if (distance_x > 0)
            z->dx = -z->velocity;
        else 
            z->dx = z->velocity;   
    }

    if (abs(distance_y) > 35) {
        if (distance_y > 0)
            z->dy = -z->velocity;
        else 
            z->dy = z->velocity;   
    }

    if (abs(distance_x) <= 35 && abs(distance_y) <= 35) {
        z->state = ZOMBIE_STATE_STAND;
        z->dx = FIX16(ZOMBIE_DEFAULT_DX);
        z->dy = FIX16(ZOMBIE_DEFAULT_DY);
    } else {
        z->state = ZOMBIE_STATE_WALK;
    }
}

void zombie_attack(struct zombie *z) {
    
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