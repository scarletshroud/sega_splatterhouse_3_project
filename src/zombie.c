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

struct distance {
    int8_t distance_x;
    int8_t distance_y;
};

static void init(struct zombie *z, const fix16 start_pos_x, const fix16 start_pos_y) {
    z->id = (global_id++);
    z->pos_x = to_game_pos_x(start_pos_x, ZOMBIE_SPRITE_WIDTH); 
    z->pos_y = to_game_pos_y(start_pos_y, ZOMBIE_SPRITE_HEIGHT);
    z->dx = FIX16(ZOMBIE_DEFAULT_DX); 
    z->dy = FIX16(ZOMBIE_DEFAULT_DY);
    z->velocity = FIX16(ZOMBIE_VELOCITY); 
    z->health = ZOMBIE_DEFAULT_HEALTH; 
    z->state = ZOMBIE_STATE_STAND;
    z->sprite = SPR_addSprite(&zombie_sprite, fix16ToInt(start_pos_x), fix16ToInt(start_pos_y), TILE_ATTR(PAL2, 0, FALSE, FALSE));
    z->width = ZOMBIE_SPRITE_WIDTH;
    z->height = ZOMBIE_SPRITE_HEIGHT;
    z->frame_counter = 0;  
}

struct zombie* create_zombie(const fix16 start_pos_x, const fix16 start_pos_y) {
    struct zombie* z = malloc(sizeof(struct zombie)); 
    init(z, start_pos_x, start_pos_y); 
    return z; 
}

static bool walk(struct zombie *z, int8_t distance_to_player) {
    const struct player_position player_pos = player_get_position(); 
    const int8_t distance_x = fix16ToInt(z->pos_x) - fix16ToInt(player_pos.x);
    const int8_t distance_y = fix16ToInt(z->pos_y) - fix16ToInt(player_pos.y); 

    if (abs(distance_x) > distance_to_player) {
        z-> dx = (distance_x > 0) ? -z->velocity : z->velocity;
    }

    if (abs(distance_y) > distance_to_player) {
        z->dy = (distance_y > 0) ? -z->velocity : z->velocity;  
    }

    if (abs(distance_x) <= distance_to_player && abs(distance_y) <= distance_to_player) {
        z->state = ZOMBIE_STATE_STAND;
        z->dx = FIX16(ZOMBIE_DEFAULT_DX);
        z->dy = FIX16(ZOMBIE_DEFAULT_DY);
        return TRUE;
    } else {
        z->state = ZOMBIE_STATE_WALK;
    }

    return FALSE;
}

#define IMPACT_DISTANCE 30
static void hit(struct zombie *z) {
    if (z->state != ZOMBIE_STATE_STAND && z->state != ZOMBIE_STATE_WALK) {
        return; 
    }

    const struct player_position player_pos = player_get_position();
    if (abs(fix16ToInt(z->pos_x) - fix16ToInt(player_pos.x)) <= IMPACT_DISTANCE) {
        z->state = ZOMBIE_STATE_HIT; 
    }
}

#define DISTANCE_TO_HIT 25
#define DISTANCE_TO_ATTACK 50
static void attack(struct zombie* z) {
    
    while(!walk(z, DISTANCE_TO_HIT)) {
    }

    hit(z);

    while(move_away(z, DISTANCE_TO_ATTACK)) {
    }
}

#define ANIM_BANG_TIME 10
#define ANIM_HIT_TIME 15
#define ANIM_DIE_TIME 30
static void handle_state(struct zombie* z) {
    switch (z->state) {
        case ZOMBIE_STATE_STAND:
        case ZOMBIE_STATE_WALK:
            walk(z, DISTANCE_TO_ATTACK);
            z->pos_x = fix16Add(z->pos_x, z->dx);
            z->pos_y = fix16Add(z->pos_y, z->dy);
            SPR_setPosition(z->sprite, fix16ToInt(to_sprite_pos_x(z->pos_x, ZOMBIE_SPRITE_WIDTH)), fix16ToInt(to_sprite_pos_y(z->pos_y, ZOMBIE_SPRITE_HEIGHT)));
            break;

        case ZOMBIE_STATE_BANG:
            if (z->frame_counter == ANIM_BANG_TIME) {
                z->state = ZOMBIE_STATE_STAND; 
                z->frame_counter = 0;
            }
            z->frame_counter++;
            break;

        case ZOMBIE_STATE_HIT:
            if (z->frame_counter == ANIM_HIT_TIME) { 
                z->state = ZOMBIE_STATE_STAND;
                z->frame_counter = 0;
            }
            z->frame_counter++; 
            break;

        case ZOMBIE_STATE_DIE:
            if (z->frame_counter == ANIM_DIE_TIME) {
                z->state = ZOMBIE_STATE_NONE;
                z->frame_counter = 0;
            }
            z->frame_counter++;
            break;

        case ZOMBIE_STATE_NONE:
            break;
    }
}

static void animate(struct zombie *z) {
    SPR_setAnim(z->sprite, z->state);
}

void zombie_update(struct zombie *z) {
    //zombie_attack(z);
    handle_state(z); 

    animate(z);
}

void bang_zombie(struct zombie *z, const int8_t damage) {
    if (z->state != ZOMBIE_STATE_BANG) {
        if (z->health <= 0) {
            z->state = ZOMBIE_STATE_DIE;
        } else {
            z->state = ZOMBIE_STATE_BANG;
            z->health = z->health - damage;
        }
    }
}