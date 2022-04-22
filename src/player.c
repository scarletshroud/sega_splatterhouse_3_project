#include "zombie_list.h"
#include "zombie.h"
#include "player.h"
#include "resources.h"
#include "play_state.h"
#include "position_util.h"

#define SPRITE_WIDTH 48
#define SPRITE_HEIGHT 80

static struct player {
    uint16_t health;
    uint16_t energy;
    uint16_t lifes;
    fix16 dx; 
    fix16 dy; 
    fix16 pos_x; 
    fix16 pos_y;
    fix16 velocity; 
    Sprite *sprite; 
    unsigned short sprite_width; 
    unsigned short sprite_height; 
    enum PLAYER_STATE state; 
    enum PLAYER_MOVE_DIRECTION direction;
};

static struct player p;
static const fix16 gravity = FIX16(0.3); 
short jumping_point;

static uint8_t frame_timer;

#define DEFAULT_DX 0 
#define DEFAULT_DY 0
#define DEFAULT_RAGE 0
#define DEFAULT_LIFE 3 
#define DEFAULT_HEALTH 100
#define DEFAULT_VELOCITY 2
#define JUMP_VELOCITY 6
void player_init(const fix16 pos_x, const fix16 pos_y) {
    p.pos_x = to_game_pos_x(pos_x, SPRITE_WIDTH); 
    p.pos_y = to_game_pos_y(pos_y, SPRITE_HEIGHT);
    p.energy = DEFAULT_RAGE;  
    p.lifes = DEFAULT_LIFE;
    p.health = DEFAULT_HEALTH;
    p.dx = FIX16(DEFAULT_DX); 
    p.dy = FIX16(DEFAULT_DY);
    p.velocity = FIX16(DEFAULT_VELOCITY);
    p.sprite = SPR_addSprite(&rick, fix16ToInt(pos_x), fix16ToInt(pos_y), TILE_ATTR(PAL1, 0, FALSE, FALSE)); 
    p.sprite_width = SPRITE_WIDTH; 
    p.sprite_height = SPRITE_HEIGHT;
    p.state = STATE_STAND;
    p.direction = DIRECTION_NONE;
    frame_timer = 0;
}

#define SCREEN_LEFT_SIDE -50
#define SCREEN_RIGHT_SIDE 250
static bool check_player_collision() {
    return (fix16ToInt(p.pos_x) - p.sprite_width / 2  + fix16ToInt(p.dx) > SCREEN_LEFT_SIDE 
            && fix16ToInt(p.pos_x) + p.sprite_width / 2 + fix16ToInt(p.dx) < SCREEN_RIGHT_SIDE);
}

static void player_walk() {
    switch (p.direction) {
        case DIRECTION_UP:
            p.dy = -p.velocity;
            p.state = STATE_WALK;
            break;

        case DIRECTION_DOWN:
            p.dy = p.velocity;
            p.state = STATE_WALK;
            break;

        case DIRECTION_RIGHT:
            p.dx = p.velocity; 
            p.state = STATE_WALK;
            break;

        case DIRECTION_LEFT: 
            p.dx = -p.velocity;
            p.state = STATE_WALK; 
            break;

        case DIRECTION_NONE:
            p.dx = FIX16(0);
            p.dy = FIX16(0);
            p.state = STATE_STAND; 
            break;
    }
}

static void player_jump() {
    if (jumping_point == 0) {
        jumping_point = fix16ToInt(p.pos_y) + p.sprite_height / 2; 
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

#define DAMAGE 25
static void player_attack() {
    struct zombie_list* list = head;
    while (list != NULL) {
        if (check_hit(list->z)) {
            bang_zombie(list->z, DAMAGE); 
        }
        list = list->next; 
    }
}

static void check_player_floor_collision() {
    if (fix16ToInt(p.pos_y) + p.sprite_height / 2 > jumping_point) {
        p.dy = FIX16(0);
        p.pos_y = FIX16(jumping_point - p.sprite_height / 2);
        jumping_point = 0;
        p.state = STATE_STAND; 
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

static void handle_player_state() {
    switch(p.state) {
        case STATE_STAND: 
        case STATE_WALK:
            player_walk();
            break; 

        case STATE_JUMP:
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

        case STATE_HIT:
            player_attack();

            if (frame_timer == 10) {
                p.state = STATE_STAND;
                frame_timer = 0; 
            }

            frame_timer++;
            break;

        case STATE_JUMP_HIT:
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

            if (frame_timer == 10) {
                p.state = STATE_JUMP;
                frame_timer = 0;
            }

            frame_timer++;
            break;
    }
}

void player_update() {
    handle_player_state();

    if (check_player_collision()) { 
        p.pos_x = fix16Add(p.pos_x, p.dx);
    }

    p.pos_y = fix16Add(p.pos_y, p.dy);

    animate_player();  
    SPR_setPosition(p.sprite, fix16ToInt(to_sprite_pos_x(p.pos_x, SPRITE_WIDTH)), fix16ToInt(to_sprite_pos_y(p.pos_y, SPRITE_HEIGHT)));
}

void player_set_direction(enum PLAYER_MOVE_DIRECTION direction) {
    p.direction = direction; 
}

void player_set_state(enum PLAYER_STATE state) {
    if (p.state == STATE_JUMP && state == STATE_HIT) {
        p.state = STATE_JUMP_HIT;
        return; 
    } 
    
    if (state == STATE_JUMP) {
        if  ((p.state == STATE_STAND || p.state == STATE_WALK)) {
            p.state = state; 
        }
        return;
    }

    p.state = state;
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