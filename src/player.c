#include "player.h"
#include "resources.h"

#define DEFAULT_DX 0 
#define DEFAULT_DY 0
#define DEFAULT_RAGE 0
#define DEFAULT_LIFES 3 
#define DEFAULT_HEALTH 100
#define DEFAULT_VELOCITY 2
#define JUMP_VELOCITY 6
#define SPRITE_WIDTH 48
#define SPRITE_HEIGHT 80

static enum STATE 
{
    STATE_STAND,
    STATE_WALK,
    STATE_JUMP,
    STATE_HIT  
}; 

static enum ANIM 
{
    ANIM_STAND,
    ANIM_WALK,
    ANIM_JUMP,
    ANIM_HIT
};

static struct player 
{
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
    enum ANIM anim;  
}; 

static struct player p;
static const fix16 gravity = FIX16(0.3); 
short jumping_point;

static uint8_t frame_timer;

void animate_player(); 
void check_player_floor_collision();

fix16 to_game_pos_x(const fix16 pos_x) {
    return fix16Add(pos_x, FIX16(SPRITE_WIDTH / 2));  
}

fix16 to_game_pos_y(const fix16 pos_y) {
    return fix16Add(pos_y, FIX16(SPRITE_HEIGHT / 2));  
}

fix16 to_sprite_pos_x(const fix16 pos_x) {
    return fix16Sub(pos_x, FIX16(SPRITE_WIDTH / 2)); 
}

fix16 to_sprite_pos_y(const fix16 pos_y) {
    return fix16Sub(pos_y, FIX16(SPRITE_HEIGHT / 2));
}

void player_init(const fix16 pos_x, const fix16 pos_y)
 {
    p.pos_x = to_game_pos_x(pos_x); 
    p.pos_y = to_game_pos_y(pos_y); 
    p.rage = DEFAULT_RAGE;  
    p.lifes = DEFAULT_LIFES;
    p.health = DEFAULT_HEALTH;  
    p.dx = FIX16(DEFAULT_DX); 
    p.dy = FIX16(DEFAULT_DY);
    p.velocity = FIX16(DEFAULT_VELOCITY);
    p.sprite = SPR_addSprite(&rick, fix16ToInt(p.pos_x), fix16ToInt(p.pos_y), TILE_ATTR(PAL1, 0, FALSE, FALSE)); 
    p.sprite_width = SPRITE_WIDTH; 
    p.sprite_height = SPRITE_HEIGHT;
    p.state = STATE_STAND;

    frame_timer = 0;
}

void update_player() 
{  
    if (fix16ToInt(p.pos_x) >= 0 && fix16ToInt(p.pos_x) < 300)
        p.pos_x = fix16Add(p.pos_x, p.dx);

    if (p.state == STATE_JUMP) 
    {
        p.dy = fix16Add(p.dy, gravity);
        check_player_floor_collision();
    }

    p.pos_y = fix16Add(p.pos_y, p.dy);

    if (frame_timer == 10) {
        frame_timer = 0;
        p.state = STATE_STAND; 
    }

    if (p.state == STATE_HIT) {
        frame_timer++;
    }

    animate_player();  
    SPR_setPosition(p.sprite, fix16ToInt(to_sprite_pos_x(p.pos_x)), fix16ToInt(to_sprite_pos_y(p.pos_y))); 
}

void player_move(enum MOVE_DIRECTION direction) 
{
    switch (p.state) 
    {   
        case STATE_STAND: 
        case STATE_WALK: 
            switch (direction)
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
            break;

        case STATE_JUMP:
            switch (direction)
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
            break; 
    }
}

void player_jump() 
{
    if (p.state != STATE_JUMP) 
    {
        p.state = STATE_JUMP; 
        jumping_point = fix16ToInt(p.pos_y) + p.sprite_height / 2; 
        p.dy = FIX16(-JUMP_VELOCITY);
    }
}

void player_hit(struct zombie *z) 
{
    p.state = STATE_HIT;

    if (fix16ToInt(p.pos_x) + p.sprite_width / 2 >= fix16ToInt(z->pos_x)) {
        bang_zombie(z, (uint8_t) 25); 
    }
}

struct player_position get_player_position() {
    return (struct player_position) {.x = p.pos_x, .y = p.pos_y};
}

void animate_player()
{
    if (p.dx > 0) {
        SPR_setHFlip(p.sprite, FALSE);
    }

    if (p.dx < 0) {
        SPR_setHFlip(p.sprite, TRUE);
    }

    switch (p.state) {
        case STATE_STAND:
            SPR_setAnim(p.sprite, ANIM_STAND);
            break;
        case STATE_WALK: 
            SPR_setAnim(p.sprite, ANIM_WALK); 
            break;
        case STATE_JUMP:
            SPR_setAnim(p.sprite, ANIM_JUMP); 
            break;
        case STATE_HIT:
            SPR_setAnim(p.sprite, ANIM_HIT);
            break;
    }
}

void check_player_floor_collision() 
{
    if (fix16ToInt(p.pos_y) + p.sprite_height / 2 > jumping_point) 
    {
        p.dy = FIX16(0);
        p.pos_y = FIX16(jumping_point - p.sprite_height / 2);
        jumping_point = 0;
        p.state = STATE_STAND; 
    }
}