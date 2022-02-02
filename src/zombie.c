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

static void zombie_init(struct zombie *z, const fix16 start_pos_x, const fix16 start_pos_y);
static void zombie_hit(struct zombie *z); 
static void zombie_walk(struct zombie *z);
static void zombie_attack(struct zombie *z); 
static void zombie_animate(struct zombie *z);

struct zombie* create_zombie(const fix16 start_pos_x, const fix16 start_pos_y) {
    struct zombie* z = (struct zombie*)malloc(sizeof(struct zombie)); 
    zombie_init(z, start_pos_x, start_pos_y); 
    return z; 
}

void zombie_init(struct zombie *z, const fix16 start_pos_x, const fix16 start_pos_y) {
    z->pos_x = to_game_pos_x(start_pos_x, ZOMBIE_SPRITE_WIDTH); 
    z->pos_y = to_game_pos_y(start_pos_y, ZOMBIE_SPRITE_HEIGHT);
    z->dx = FIX16(ZOMBIE_DEFAULT_DX); 
    z->dy = FIX16(ZOMBIE_DEFAULT_DY);
    z->velocity = FIX16(ZOMBIE_VELOCITY); 
    z->health = ZOMBIE_DEFAULT_HEALTH; 
    z->state = ZOMBIE_STATE_STAND;
    z->sprite = SPR_addSprite(&zombie_sprite, fix16_to_int(start_pos_x), fix16_to_int(start_pos_y), TILE_ATTR(PAL2, 0, FALSE, FALSE));
    z->width = ZOMBIE_SPRITE_WIDTH;
    z->height = ZOMBIE_SPRITE_HEIGHT;
    z->frame_counter = 0;  
}

void zombie_update(struct zombie *z) {
    //zombie_hit(z);
    switch (z->state) {
        case ZOMBIE_STATE_STAND:
        case ZOMBIE_STATE_WALK:
            //zombie_walk(z);
            z->pos_x = fix16_add(z->pos_x, z->dx);
            z->pos_y = fix16_add(z->pos_y, z->dy);
            SPR_setPosition(z->sprite, fix16_to_int(to_sprite_pos_x(z->pos_x, ZOMBIE_SPRITE_WIDTH)), fix16_to_int(to_sprite_pos_y(z->pos_y, ZOMBIE_SPRITE_HEIGHT)));
            break;

        case ZOMBIE_STATE_BANG:
            if (z->frame_counter == 10) {
                z->state = ZOMBIE_STATE_STAND; 
                z->frame_counter = 0;
            }
            z->frame_counter++;
            break;

        case ZOMBIE_STATE_HIT:
            if (z->frame_counter == 15) { 
                z->state = ZOMBIE_STATE_STAND;
                z->frame_counter = 0;
            }
            z->frame_counter++; 
            break;

        case ZOMBIE_STATE_DIE:
            if (z->frame_counter == 30) {
                z->state = ZOMBIE_STATE_NONE;
                z->frame_counter = 0;
            }
            z->frame_counter++;
            break;

        case ZOMBIE_STATE_NONE:
            break;
    }

    zombie_animate(z);
}

#define IMPACT_DISTANCE 60
void zombie_hit(struct zombie *z) {
    if (z->state != ZOMBIE_STATE_STAND && z->state != ZOMBIE_STATE_WALK) {
        return; 
    }

    const struct player_position player_pos = get_player_position();
    if (abs(fix16_to_int(z->pos_x) - fix16_to_int(player_pos.x)) <= IMPACT_DISTANCE) {
        z->state = ZOMBIE_STATE_HIT; 
    }
}

#define DISTANCE_TO_HIT 35
void zombie_walk(struct zombie *z) {
    const struct player_position player_pos = get_player_position(); 
    const int8_t distance_x = fix16_to_int(z->pos_x) - fix16_to_int(player_pos.x);
    const int8_t distance_y = fix16_to_int(z->pos_y) - fix16_to_int(player_pos.y); 

    if (abs(distance_x) > DISTANCE_TO_HIT) {
        z-> dx = (distance_x > 0) ? -z->velocity : z->velocity;
    }

    if (abs(distance_y) > DISTANCE_TO_HIT) {
        z->dy = (distance_y > 0) ? -z->velocity : z->velocity;  
    }

    if (abs(distance_x) <= DISTANCE_TO_HIT && abs(distance_y) <= DISTANCE_TO_HIT) {
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
    SPR_setAnim(z->sprite, z->state);
}

void bang_zombie(struct zombie *z, const uint8_t damage) {
    if (z->health <= 0) {
        z->state = ZOMBIE_STATE_DIE;
    } else {
        z->state = ZOMBIE_STATE_BANG;
        z->health = z->health - damage;
    }
}