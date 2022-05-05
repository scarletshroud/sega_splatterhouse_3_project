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
    int16_t health;
    uint16_t energy;
    uint16_t lifes;
    uint16_t damage;
    fix16 dx; 
    fix16 dy; 
    fix16 pos_x; 
    fix16 pos_y;
    fix16 velocity_x;
    fix16 velocity_y; 
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
#define DEFAULT_VELOCITY_X 2
#define DEFAULT_VELOCITY_Y 1
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
    p.velocity_x = FIX16(DEFAULT_VELOCITY_X);
    p.velocity_y = FIX16(DEFAULT_VELOCITY_Y);
    p.sprite = SPR_addSprite(&rick, fix16ToInt(pos_x), fix16ToInt(pos_y), TILE_ATTR(PAL1, 0, FALSE, FALSE)); 
    p.sprite_width = SPRITE_WIDTH; 
    p.sprite_height = SPRITE_HEIGHT;
    p.jumping_point = 0;
    p.state = STATE_STAND;
    p.direction = DIRECTION_NONE;
    p.beast_mode = FALSE;

    timer_reset(&frame_timer);
    timer_reset(&beast_mode_timer);

    XGM_setPCM(64, bump_sound, sizeof(bump_sound));
    XGM_setPCM(65, rick_death_sound, sizeof(rick_death_sound));
}

static bool check_axis_x_collision() {
    return (fix16ToInt(p.pos_x) - p.sprite_width / 2  + fix16ToInt(p.dx) > SCREEN_LEFT_SIDE 
            && fix16ToInt(p.pos_x) + p.sprite_width / 2 + fix16ToInt(p.dx) < SCREEN_RIGHT_SIDE);
}

static bool check_axis_y_collision() {
    return (fix16ToInt(p.pos_y) + p.sprite_height / 2  + fix16ToInt(p.dy) > FLOOR_UPPER_SIDE 
            && fix16ToInt(p.pos_y) + p.sprite_height / 2 + fix16ToInt(p.dy) < FLOOR_DOWN_SIDE);
}

static void update_state(enum PLAYER_STATE state) {
    p.state = state + p.beast_mode * BEAST_STATE_OFFSET;
}

static void walk() {
    switch (p.direction) {
        case DIRECTION_UP:
            p.dy = -p.velocity_y;
            update_state(STATE_WALK);
            break;

        case DIRECTION_DOWN:
            p.dy = p.velocity_y;
            update_state(STATE_WALK);
            break;

        case DIRECTION_RIGHT:
            p.dx = p.velocity_x; 
            update_state(STATE_WALK);
            break;

        case DIRECTION_LEFT: 
            p.dx = -p.velocity_x;
            update_state(STATE_WALK); 
            break;

        case DIRECTION_NONE:
            p.dx = FIX16(0);
            p.dy = FIX16(0);
            update_state(STATE_STAND); 
            break;
    }
}

static void jump() {
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

static void attack() {
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
            XGM_startPlayPCM(64, 15, SOUND_PCM_CH2); 
        }
    }
}

static bool check_floor_collision() {
    if (fix16ToInt(p.pos_y) + p.sprite_height / 2 > p.jumping_point) {
        p.dy = FIX16(0);
        p.pos_y = FIX16(p.jumping_point - p.sprite_height / 2);
        p.jumping_point = 0;
        return TRUE;
    }
    return FALSE;
}

static void horizontal_rotate() {
    if (p.dx != 0) {
        bool h_flip = (p.dx > FIX16(0)) ? FALSE : TRUE; 
        SPR_setHFlip(p.sprite, h_flip); 
    }
}

static void animate() {      
    horizontal_rotate(); 
    SPR_setAnim(p.sprite, p.state);
}

static void transform_to_normal() {
    p.damage = DEFAULT_DAMAGE;
    p.beast_mode = FALSE;
    timer_reset(&frame_timer);
    timer_reset(&beast_mode_timer);
}

static void rebel() {
    p.health = 100;
    p.lifes--;
    p.state = STATE_STAND;
}

static void handle_jump_direction() {
    switch (p.direction) {
        case DIRECTION_RIGHT:
            p.dx = p.velocity_x; 
            break;

        case DIRECTION_LEFT: 
            p.dx = -p.velocity_x;
            break;

        case DIRECTION_NONE:
            p.dx = FIX16(0);
            break;
    }
}

static void fall_down() {
    p.dy = fix16Add(p.dy, gravity);
    if (check_floor_collision()) {
        update_state(STATE_STAND); 
    }
}

#define BEAST_MODE_TIME_LIMIT 500
static void handle_state() {
    switch(p.state) {

        case STATE_STAND:
        case STATE_BEAST_STAND: 
        case STATE_WALK:
        case STATE_BEAST_WALK:
            walk();
            break; 

        case STATE_JUMP:
        case STATE_BEAST_JUMP:
            jump();
            handle_jump_direction();
            fall_down();
            break;

        case STATE_FIRST_HIT:
        case STATE_SECOND_HIT:
        case STATE_THIRD_HIT:
        case STATE_BEAST_FIRST_HIT:
        case STATE_BEAST_SECOND_HIT:
        case STATE_BEAST_THIRD_HIT:

            if (frame_timer.time == 15) {
                attack();
                update_state(STATE_STAND); 
                timer_reset(&frame_timer); 
            }

            timer_tick(&frame_timer);
            break;

        case STATE_JUMP_HIT:
        case STATE_BEAST_JUMP_HIT:
            attack();
            handle_jump_direction();
            fall_down();
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

        case STATE_BANG:
            if (frame_timer.time == 7) {
                update_state(STATE_STAND);
                timer_reset(&frame_timer);
            }
            timer_tick(&frame_timer);
            break;

        case STATE_DIE:
            if (frame_timer.time == 35) {

                if (p.lifes > 0) {
                    rebel();
                } else {
                    XGM_startPlayPCM(65, 15, SOUND_PCM_CH2); 
                    p.state = STATE_DEAD;
                }

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
    if (check_axis_x_collision()) { 
        p.pos_x = fix16Add(p.pos_x, p.dx);
    }

    if (check_axis_y_collision() || p.state == STATE_JUMP || p.state == STATE_JUMP_HIT || p.state == STATE_BEAST_JUMP || p.state == STATE_BEAST_JUMP_HIT) {
        p.pos_y = fix16Add(p.pos_y, p.dy);
    }
}

static void check_health() {
    if (p.health == 0) {
        p.state = STATE_DIE;
    }
}

void player_update() {
    handle_state();
    update_position();

    if (p.beast_mode)
        timer_tick(&beast_mode_timer);

    check_health();

    animate();  
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

void player_bang(int8_t damage) {
    p.health -= damage;
    update_state(STATE_BANG);
    KLog_U1("hm state ", p.state);
}

enum PLAYER_MOVE_DIRECTION player_get_direction() {
    return p.direction; 
}

enum PLAYER_STATE player_get_state() {
    return p.state;
}

struct Sprite* player_get_sprite() {
    return p.sprite;
}

int16_t player_get_health() {
    return p.health;
}

uint16_t player_get_energy() {
    return p.energy;
}

uint16_t player_get_lifes() {
    return p.lifes;
}

struct player_position player_get_position() {
    return (struct player_position) {.x = p.pos_x, .y = p.pos_y};
}