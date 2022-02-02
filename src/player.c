#include "player.h"
#include "resources.h"

static enum STATE {
    STATE_STAND,
    STATE_WALK,
    STATE_JUMP,
    STATE_HIT  
}; 

#define SPRITE_WIDTH 48
#define SPRITE_HEIGHT 80

static struct player {
    uint8_t health;
    uint8_t rage;
    uint8_t lifes;
    fix16 dx; 
    fix16 dy; 
    fix16 pos_x; 
    fix16 pos_y;
    fix16 velocity; 
    Sprite *sprite; 
    unsigned short sprite_width; 
    unsigned short sprite_height; 
    enum STATE state; 
    enum STATE direction;
}; 

static struct player p;
static const fix16 gravity = FIX16(0.3); 
short jumping_point;

static uint8_t frame_timer;

static void animate_player(); 
static void check_player_floor_collision();

#define DEFAULT_DX 0 
#define DEFAULT_DY 0
#define DEFAULT_RAGE 0
#define DEFAULT_LIFES 3 
#define DEFAULT_HEALTH 100
#define DEFAULT_VELOCITY 2
#define JUMP_VELOCITY 6

void player_init(const fix16 pos_x, const fix16 pos_y) {
    p.pos_x = to_game_pos_x(pos_x, SPRITE_WIDTH); 
    p.pos_y = to_game_pos_y(pos_y, SPRITE_HEIGHT);
    p.rage = DEFAULT_RAGE;  
    p.lifes = DEFAULT_LIFES;
    p.health = DEFAULT_HEALTH;
    p.dx = FIX16(DEFAULT_DX); 
    p.dy = FIX16(DEFAULT_DY);
    p.velocity = FIX16(DEFAULT_VELOCITY);
    p.sprite = SPR_addSprite(&rick, fix16_to_int(pos_x), fix16_to_int(pos_y), TILE_ATTR(PAL1, 0, FALSE, FALSE)); 
    p.sprite_width = SPRITE_WIDTH; 
    p.sprite_height = SPRITE_HEIGHT;
    p.state = STATE_STAND;
    p.direction = DIRECTION_NONE;
    frame_timer = 0;
}

#define SCREEN_LEFT_SIDE 0
#define SCREEN_RIGHT_SIDE 300
static void check_player_collision() {
    if (fix16_to_int(p.pos_x) - p.sprite_width / 2  + fix16_to_int(p.dx) > SCREEN_LEFT_SIDE && fix16_to_int(p.pos_x) + p.sprite_width / 2 + fix16_to_int(p.dx) < SCREEN_RIGHT_SIDE) {
        p.pos_x = fix16_add(p.pos_x, p.dx);
    }   
}

static void player_walk() {
    
    switch (p.direction)
    {
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

void player_update() {
    check_player_collision(); 

    switch(p.state) {
        case STATE_STAND: 
        case STATE_WALK:
            player_walk();
            break; 

        case STATE_HIT:
            if (frame_timer == 10) {
                frame_timer = 0;
                p.state = STATE_STAND; 
            }
            frame_timer++;
            break;

        case STATE_JUMP:
            switch (p.direction)
            {
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
            p.dy = fix16_add(p.dy, gravity);
            check_player_floor_collision();
            p.pos_y = fix16_add(p.pos_y, p.dy);
            break;
    }

    animate_player();  
    SPR_setPosition(p.sprite, fix16_to_int(to_sprite_pos_x(p.pos_x, SPRITE_WIDTH)), fix16_to_int(to_sprite_pos_y(p.pos_y, SPRITE_HEIGHT)));
}

/*void player_update() {  
    check_player_collision(); 

    if (p.state == STATE_JUMP) {
        p.dy = fix16_add(p.dy, gravity);
        check_player_floor_collision();
    }

    p.pos_y = fix16_add(p.pos_y, p.dy);

    if (frame_timer == 10) {
        frame_timer = 0;
        p.state = STATE_STAND; 
    }

    if (p.state == STATE_HIT) {
        frame_timer++;
    }

    KLog_U1("player state: ", p.state);
    animate_player();  
    SPR_setPosition(p.sprite, fix16_to_int(to_sprite_pos_x(p.pos_x, SPRITE_WIDTH)), fix16_to_int(to_sprite_pos_y(p.pos_y, SPRITE_HEIGHT))); 
} */

void player_jump() {
    if (p.state != STATE_JUMP) {
        p.state = STATE_JUMP; 
        jumping_point = fix16_to_int(p.pos_y) + p.sprite_height / 2; 
        p.dy = FIX16(-JUMP_VELOCITY);
    }
}

static bool check_hit_axis_y(const struct zombie* const z) {
    return (fix16_to_int(p.pos_y) >= fix16_to_int(z->pos_y) - z->height / 2 && fix16_to_int(p.pos_y) <= fix16_to_int(z->pos_y) + z->height / 2);
}

static bool check_hit_axis_x(const struct zombie* const z) {
    return (fix16_to_int(p.pos_x) + p.sprite_width / 2 >= fix16_to_int(z->pos_x) - z->width / 2 && fix16_to_int(p.pos_x) + p.sprite_width / 2 <= fix16_to_int(z->pos_x) + z->width / 2
                || fix16_to_int(p.pos_x) - p.sprite_width / 2 >= fix16_to_int(z->pos_x) - z->width / 2 && fix16_to_int(p.pos_x) - p.sprite_width / 2 <= fix16_to_int(z->pos_x) + z->width / 2);
}

static bool check_hit(const struct zombie* const z) {
    return (check_hit_axis_y(z) && check_hit_axis_x(z));  
}

#define DAMAGE 25
void player_attack(struct zombie_list* list) {
    p.state = STATE_HIT;
    while (list != NULL) {
        if (check_hit(list->z)) {
            bang_zombie(list->z, DAMAGE); 
        }
        list = list->next; 
    }
}

void player_set_direction(enum MOVE_DIRECTION direction) {
    p.direction = direction; 
}

struct player_position get_player_position() {
    return (struct player_position) {.x = p.pos_x, .y = p.pos_y};
}

static void rotate_player() {
    if (p.dx != 0) {
        bool h_flip = (p.dx > FIX16(0)) ? FALSE : TRUE; 
        SPR_setHFlip(p.sprite, h_flip); 
    }
}

void animate_player() {      
    rotate_player(); 
    SPR_setAnim(p.sprite, p.state);
}

void check_player_floor_collision() {
    if (fix16_to_int(p.pos_y) + p.sprite_height / 2 > jumping_point) {
        p.dy = FIX16(0);
        p.pos_y = FIX16(jumping_point - p.sprite_height / 2);
        jumping_point = 0;
        p.state = STATE_STAND; 
    }
}