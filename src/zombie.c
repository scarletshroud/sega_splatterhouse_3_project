#include "zombie.h"
#include "resources.h"
#include "play_state.h"
#include "timer.h"

#define ZOMBIE_VELOCITY 2
#define ZOMBIE_DEFAULT_DX 0 
#define ZOMBIE_DEFAULT_DY 0 
#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 64   
#define DEFAULT_HEALTH 100
#define DEFAULT_DAMAGE 25

enum ZOMBIE_ANIM {
    ANIM_STAND,
    ANIM_WALK,
    ANIM_HIT,
    ANIM_BANG,
    ANIM_DIE
};

uint16_t frame_timer = 0;

#define DISTANCE_TO_HIT 25
#define DISTANCE_TO_ATTACK 70

static void init(struct zombie *z, const fix16 start_pos_x, const fix16 start_pos_y) {
    z->id = (global_id++);
    z->pos_x = to_game_pos_x(start_pos_x, SPRITE_WIDTH); 
    z->pos_y = to_game_pos_y(start_pos_y, SPRITE_HEIGHT);
    z->dx = FIX16(ZOMBIE_DEFAULT_DX); 
    z->dy = FIX16(ZOMBIE_DEFAULT_DY);
    z->velocity = FIX16(ZOMBIE_VELOCITY); 
    z->health = DEFAULT_HEALTH;
    z->damage = DEFAULT_DAMAGE; 
    z->state = ZOMBIE_STATE_STAND;
    z->sprite = SPR_addSprite(&zombie_sprite, fix16ToInt(start_pos_x), fix16ToInt(start_pos_y), TILE_ATTR(PAL2, 0, FALSE, FALSE));
    z->width = SPRITE_WIDTH;
    z->height = SPRITE_HEIGHT;
    z->target_distance = DISTANCE_TO_ATTACK;
    z->frame_counter = 0;  
}

struct zombie* create_zombie(const fix16 start_pos_x, const fix16 start_pos_y) {
    struct zombie* z = malloc(sizeof(struct zombie)); 
    init(z, start_pos_x, start_pos_y); 
    return z; 
}

static int8_t calculate_distance_x_to_player(const struct zombie* const z, const struct player_position* const player_pos) {
    return fix16ToInt(z->pos_x) - fix16ToInt(player_pos->x);
}

static int8_t calculate_distance_y_to_player(const struct zombie* const z, const struct player_position* const player_pos) {
    return fix16ToInt(z->pos_y) - fix16ToInt(player_pos->y);
}

static bool check_axis_x_collision(const struct zombie* const z) {
    return !(fix16ToInt(z->pos_x) - z->width / 2  + fix16ToInt(z->dx) > SCREEN_LEFT_SIDE 
            && fix16ToInt(z->pos_x) + z->width / 2 + fix16ToInt(z->dx) < SCREEN_RIGHT_SIDE);
}

static bool check_axis_y_collision(const struct zombie* const z) {
    return !(fix16ToInt(z->pos_y) + z->height / 2  + fix16ToInt(z->dy) > FLOOR_UPPER_SIDE 
            && fix16ToInt(z->pos_y) + z->height / 2 + fix16ToInt(z->dy) < FLOOR_DOWN_SIDE);
}

static void update_position(struct zombie* z) {
    
    if (!check_axis_x_collision(z)) {
        z->pos_x = fix16Add(z->pos_x, z->dx);
    }

    if (!check_axis_y_collision(z)) {
        z->pos_y = fix16Add(z->pos_y, z->dy);
    }
}

static bool walk(struct zombie *z) {
    const struct player_position player_pos = player_get_position(); 
    const int8_t distance_x = calculate_distance_x_to_player(z, &player_pos);
    const int8_t distance_y = calculate_distance_y_to_player(z, &player_pos); 

    if (abs(distance_x) > z->target_distance) {
        z->dx = (distance_x > 0) ? -z->velocity : z->velocity;
    }

    if (abs(distance_y) > z->target_distance) {
        z->dy = (distance_y > 0) ? -z->velocity : z->velocity;  
    }

    return (abs(distance_x) <= z->target_distance && abs(distance_y) <= z->target_distance);
}

static bool walk_away(struct zombie *z) {
    const struct player_position player_pos = player_get_position(); 
    const int8_t distance_x = calculate_distance_x_to_player(z, &player_pos);
    const int8_t distance_y = calculate_distance_y_to_player(z, &player_pos); 

    if (abs(distance_x) < z->target_distance) {
        z->dx = (distance_x > 0) ? z->velocity : -z->velocity;
    }

    if (abs(distance_y) < z->target_distance) {
        z->dy = (distance_y > 0) ? z->velocity : -z->velocity;  
    }

    return (abs(distance_x) >= z->target_distance && abs(distance_y) >= z->target_distance);
}

#define IMPACT_DISTANCE 30
static void hit(struct zombie *z) {
    if (z->state != ZOMBIE_STATE_STAND && z->state != ZOMBIE_STATE_WALK && z->state != ZOMBIE_STATE_ATTACK) {
        return; 
    }

    const struct player_position player_pos = player_get_position();
    if (abs(fix16ToInt(z->pos_x) - fix16ToInt(player_pos.x)) <= IMPACT_DISTANCE && abs(fix16ToInt(z->pos_y) - fix16ToInt(player_pos.y) <= IMPACT_DISTANCE)) {
        z->state = ZOMBIE_STATE_HIT;
        player_bang(z->damage);         
    }
}

static void horizontal_rotate(struct zombie* const z) {
    if (z->dx != 0) {
        bool h_flip = (z->dx > FIX16(0)) ? TRUE : FALSE; 
        SPR_setHFlip(z->sprite, h_flip); 
    }
}

static void animate(struct zombie *z, enum ZOMBIE_STATE state) {
    horizontal_rotate(z);
    SPR_setAnim(z->sprite, state);
}

#define ANIM_BANG_TIME 10
#define ANIM_HIT_TIME 15
#define ANIM_DIE_TIME 30
static void handle_state(struct zombie* z) {
    switch (z->state) {
        case ZOMBIE_STATE_STAND:
        case ZOMBIE_STATE_WALK:

            if (walk(z)) {
                z->state = ZOMBIE_STATE_STAND;
                z->dx = FIX16(ZOMBIE_DEFAULT_DX);
                z->dy = FIX16(ZOMBIE_DEFAULT_DY);
            } else {
                z->state = ZOMBIE_STATE_WALK;
                update_position(z);
            }

            animate(z, z->state);
            break;

        case ZOMBIE_STATE_BANG:
            if (z->frame_counter == ANIM_BANG_TIME) {
                z->state = ZOMBIE_STATE_STAND; 
                z->frame_counter = 0;
            }
            z->frame_counter++;
            animate(z, z->state);
            break;

        case ZOMBIE_STATE_HIT:
            if (z->frame_counter == ANIM_HIT_TIME) { 
                z->state = ZOMBIE_STATE_WALK_AWAY;
                z->target_distance = DISTANCE_TO_ATTACK;
                z->frame_counter = 0;
            }
            z->frame_counter++; 
            animate(z, z->state);
            break;

        case ZOMBIE_STATE_DIE:
            if (z->frame_counter == ANIM_DIE_TIME) {
                z->state = ZOMBIE_STATE_NONE;
                z->frame_counter = 0;
                break;
            }
            z->frame_counter++;
            animate(z, z->state);
            break;

        case ZOMBIE_STATE_ATTACK:
            if (walk(z)) {
                z->dx = FIX16(ZOMBIE_DEFAULT_DX);
                z->dy = FIX16(ZOMBIE_DEFAULT_DY);
                hit(z);
            } else {
                update_position(z);
            }

            animate(z, ZOMBIE_STATE_WALK);

            break;

        case ZOMBIE_STATE_WALK_AWAY: 
            if (walk_away(z)) {
                z->state = ZOMBIE_STATE_STAND;
                z->dx = FIX16(ZOMBIE_DEFAULT_DX);
                z->dy = FIX16(ZOMBIE_DEFAULT_DY);
            } else {
                z->state = ZOMBIE_STATE_WALK_AWAY;
                update_position(z);        
            }
            animate(z, ZOMBIE_STATE_WALK);
            break;

        case ZOMBIE_STATE_NONE:
            break;
    }
}


static void attack(struct zombie* z) {
    if (frame_timer == 200 && (z->state == ZOMBIE_STATE_STAND || z->state == ZOMBIE_STATE_WALK)) {
        z->state = ZOMBIE_STATE_ATTACK;
        z->target_distance = DISTANCE_TO_HIT;
        frame_timer = 0;
    }

    frame_timer++;
}

void zombie_update(struct zombie *z) {
    attack(z);
    handle_state(z);
    KLog_U1("zstate ", z->state);
    SPR_setPosition(z->sprite, fix16ToInt(to_sprite_pos_x(z->pos_x, SPRITE_WIDTH)), fix16ToInt(to_sprite_pos_y(z->pos_y, SPRITE_HEIGHT)));
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