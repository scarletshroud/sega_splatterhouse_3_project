#include "timer.h"
#include "zombie.h"
#include "player.h"
#include "resources.h"
#include "play_state.h"
#include "zombie_list.h"
#include "position_util.h"

#define SPRITE_WIDTH 48
#define SPRITE_HEIGHT 80
#define BEAST_STATE_OFFSET 8

static struct player {
    uint16_t health;
    uint16_t energy;
    uint16_t lifes;
    uint16_t damage;
    fix16 dx; 
    fix16 dy; 
    fix16 pos_x; 
    fix16 pos_y;
    fix16 velocity; 
    Sprite *sprite; 
    unsigned short sprite_width; 
    unsigned short sprite_height;
    unsigned short jumping_point; 
    enum PLAYER_STATE state; 
    enum PLAYER_MOVE_DIRECTION direction;
    bool beast_mode;
};

static struct player p;
static const fix16 gravity = FIX16(0.3);
 
static struct timer frame_timer;
static struct timer beast_mode_timer;

#define DEFAULT_DX 0 
#define DEFAULT_DY 0
#define DEFAULT_ENERGY 51
#define DEFAULT_LIFE 3 
#define DEFAULT_HEALTH 100
#define DEFAULT_DAMAGE 35
#define BEAST_DAMAGE 100
#define DEFAULT_VELOCITY 2
#define JUMP_VELOCITY 6
void player_init(const fix16 pos_x, const fix16 pos_y) {
    p.pos_x = to_game_pos_x(pos_x, SPRITE_WIDTH); 
    p.pos_y = to_game_pos_y(pos_y, SPRITE_HEIGHT);
    p.energy = DEFAULT_ENERGY;  
    p.lifes = DEFAULT_LIFE;
    p.health = DEFAULT_HEALTH;
    p.damage = DEFAULT_DAMAGE;
    p.dx = FIX16(DEFAULT_DX); 
    p.dy = FIX16(DEFAULT_DY);
    p.velocity = FIX16(DEFAULT_VELOCITY);
    p.sprite = SPR_addSprite(&rick, fix16ToInt(pos_x), fix16ToInt(pos_y), TILE_ATTR(PAL1, 0, FALSE, FALSE)); 
    p.sprite_width = SPRITE_WIDTH; 
    p.sprite_height = SPRITE_HEIGHT;
    p.jumping_point = 0;
    p.state = STATE_STAND;
    p.direction = DIRECTION_NONE;
    p.beast_mode = FALSE;

    timer_reset(&frame_timer);
    timer_reset(&beast_mode_timer);
}

#define SCREEN_LEFT_SIDE -50
#define SCREEN_RIGHT_SIDE 250
static bool check_player_collision() {
    return (fix16ToInt(p.pos_x) - p.sprite_width / 2  + fix16ToInt(p.dx) > SCREEN_LEFT_SIDE 
            && fix16ToInt(p.pos_x) + p.sprite_width / 2 + fix16ToInt(p.dx) < SCREEN_RIGHT_SIDE);
}

static void update_state(enum PLAYER_STATE state) {
    p.state = state + p.beast_mode * BEAST_STATE_OFFSET;
}

static void player_walk() {
    switch (p.direction) {
        case DIRECTION_UP:
            p.dy = -p.velocity;
            update_state(STATE_WALK);
            break;

        case DIRECTION_DOWN:
            p.dy = p.velocity;
            update_state(STATE_WALK);
            break;

        case DIRECTION_RIGHT:
            p.dx = p.velocity; 
            update_state(STATE_WALK);
            break;

        case DIRECTION_LEFT: 
            p.dx = -p.velocity;
            update_state(STATE_WALK); 
            break;

        case DIRECTION_NONE:
            p.dx = FIX16(0);
            p.dy = FIX16(0);
            update_state(STATE_STAND); 
            break;
    }
}

static void player_jump() {
    if (p.jumping_point == 0) {
        p.jumping_point = fix16ToInt(p.pos_y) + p.sprite_height / 2; 
        p.dy = FIX16(-JUMP_VELOCITY);
    }
}

static bool check_hit_axis_y(const struct zombie* const z) {
    return (fix16ToInt(p.pos_y) >= fix16ToInt(z->pos_y) - z->height / 2 && fix16ToInt(p.pos_y) <= fix16ToInt(z->pos_y) + z->height / 2);
}

static bool check_hit_axis_x(const struct zombie* const z) {
    return ((fix16ToInt(p.pos_x) + p.sprite_width / 2 >= fix16ToInt(z->pos_x) - z->width / 2 && fix16ToInt(p.pos_x) + p.sprite_width / 2 <= fix16ToInt(z->pos_x) + z->width / 2)
                || (fix16ToInt(p.pos_x) - p.sprite_width / 2 >= fix16ToInt(z->pos_x) - z->width / 2 && fix16ToInt(p.pos_x) - p.sprite_width / 2 <= fix16ToInt(z->pos_x) + z->width / 2));
}

static bool check_hit(const struct zombie* const z) {
    return (check_hit_axis_y(z) && check_hit_axis_x(z));  
}

static void player_attack() {
    /*
    struct zombie_list* list = head;
    while (list != NULL) {
        if (check_hit(list->z)) {
            bang_zombie(list->z, DAMAGE); 
        }
        list = list->next; 
    } */

    for (uint8_t i = 0; i < 3; ++i) {
        if (check_hit(zombies[i])) {
            bang_zombie(zombies[i], p.damage); 
        }
    }
}

static void check_player_floor_collision() {
    if (fix16ToInt(p.pos_y) + p.sprite_height / 2 > p.jumping_point) {
        p.dy = FIX16(0);
        p.pos_y = FIX16(p.jumping_point - p.sprite_height / 2);
        p.jumping_point = 0;
        update_state(STATE_STAND); 
    }
}

static void rotate_player() {
    if (p.dx != 0) {
        bool h_flip = (p.dx > FIX16(0)) ? FALSE : TRUE; 
        SPR_setHFlip(p.sprite, h_flip); 
    }
}

static void animate_player() {      
    rotate_player(); 
    SPR_setAnim(p.sprite, p.state);
}

static void transform_to_normal() {
    p.damage = DEFAULT_DAMAGE;
    p.beast_mode = FALSE;
    timer_reset(&frame_timer);
    timer_reset(&beast_mode_timer);
}

#define BEAST_MODE_TIME_LIMIT 500
static void handle_player_state() {
    switch(p.state) {

        case STATE_STAND:
        case STATE_BEAST_STAND: 
        case STATE_WALK:
        case STATE_BEAST_WALK:

            player_walk();

            break; 

        case STATE_JUMP:
        case STATE_BEAST_JUMP:

            player_jump();

            switch (p.direction) {
                case DIRECTION_RIGHT:
                    p.dx = p.velocity; 
                    break;

                case DIRECTION_LEFT: 
                    p.dx = -p.velocity;
                    break;

                case DIRECTION_NONE:
                    p.dx = FIX16(0);
                    break;
            }

            p.dy = fix16Add(p.dy, gravity);
            check_player_floor_collision();

            break;

        case STATE_FIRST_HIT:
        case STATE_SECOND_HIT:
        case STATE_THIRD_HIT:
        case STATE_BEAST_FIRST_HIT:
        case STATE_BEAST_SECOND_HIT:
        case STATE_BEAST_THIRD_HIT:

            if (frame_timer.time == 15) {
                player_attack();
                update_state(STATE_STAND); 
                timer_reset(&frame_timer); 
            }

            timer_tick(&frame_timer);
            break;

        case STATE_JUMP_HIT:
        case STATE_BEAST_JUMP_HIT:

            switch (p.direction) {
                case DIRECTION_RIGHT:
                    p.dx = p.velocity; 
                    break;

                case DIRECTION_LEFT: 
                    p.dx = -p.velocity;
                    break;

                case DIRECTION_NONE:
                    p.dx = FIX16(0);
                    break;
            }

            if (frame_timer.time == 10) {
                update_state(STATE_JUMP); 
                timer_reset(&frame_timer);
            }

            timer_tick(&frame_timer);

            break;

        case STATE_BEAST_TRANSORMATION:
            if (frame_timer.time == 30) {
                update_state(STATE_STAND);
                timer_reset(&frame_timer); 
            } 
            timer_tick(&frame_timer);
            break;
        
        case STATE_NORMAL_TRANSORMATION:
            if (frame_timer.time == 30) {
                update_state(STATE_STAND);
                timer_reset(&frame_timer); 
            } 
            timer_tick(&frame_timer);
            break;
    }

    if (beast_mode_timer.time == BEAST_MODE_TIME_LIMIT) {
        p.state = STATE_NORMAL_TRANSORMATION;
        transform_to_normal();
    } 
}

static void update_position() {
    if (check_player_collision()) { 
        p.pos_x = fix16Add(p.pos_x, p.dx);
    }

    p.pos_y = fix16Add(p.pos_y, p.dy);
}

void player_update() {
    handle_player_state();
    update_position();

    if (p.beast_mode)
        timer_tick(&beast_mode_timer);

    //KLog_U1("state ", p.state);

    animate_player();  
    SPR_setPosition(p.sprite, fix16ToInt(to_sprite_pos_x(p.pos_x, SPRITE_WIDTH)), fix16ToInt(to_sprite_pos_y(p.pos_y, SPRITE_HEIGHT)));
}

void player_set_direction(enum PLAYER_MOVE_DIRECTION direction) {
    p.direction = direction; 
}

#define ENERGY_TO_TRANSFORM 50
static void transform_to_beast() {
    p.energy = p.energy - ENERGY_TO_TRANSFORM;
    p.damage = BEAST_DAMAGE;
    p.beast_mode = TRUE;
    timer_reset(&frame_timer);
    timer_reset(&beast_mode_timer);   
}

static enum PLAYER_STATE random_hit_state() {
    return random() % (STATE_FIRST_HIT) + STATE_FIRST_HIT; //Division. 
}

void player_set_state(enum PLAYER_STATE state) {
    if (p.state == STATE_JUMP && state == STATE_FIRST_HIT) {
        update_state(STATE_JUMP_HIT); 
        return; 
    } 
    
    if (state == STATE_JUMP) {
        if  ((p.state == STATE_STAND || p.state == STATE_BEAST_STAND || p.state == STATE_WALK || p.state == STATE_BEAST_WALK)) {
            update_state(state); 
        }
        return;
    }

    if (state == STATE_BEAST_TRANSORMATION) {
        if (p.energy < ENERGY_TO_TRANSFORM) {
            return;
        }

        update_state(state);
        transform_to_beast();
        return;
    }

    if (state == STATE_FIRST_HIT) {
        if  ((p.state == STATE_STAND || p.state == STATE_BEAST_STAND || p.state == STATE_WALK || p.state == STATE_BEAST_WALK)) {
            state = random_hit_state();
            update_state(state);
        }

        return;
    }

    update_state(state);
}

enum PLAYER_MOVE_DIRECTION player_get_direction() {
    return p.direction; 
}

enum PLAYER_STATE get_player_state() {
    return p.state;
}

uint16_t get_player_health() {
    return p.health;
}

uint16_t get_player_energy() {
    return p.energy;
}

uint16_t get_player_lifes() {
    return p.lifes;
}

struct player_position get_player_position() {
    return (struct player_position) {.x = p.pos_x, .y = p.pos_y};
}