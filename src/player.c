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

#define ANIM_STAND 0
#define ANIM_WALK 1
#define ANIM_JUMP 2

enum STATE 
{
    STATE_STAND,
    STATE_WALK,
    STATE_JUMP  
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
}; 

static struct player p;
static const fix16 gravity = FIX16(0.3); 
short jumping_point;

void animate_player(); 
void check_player_floor_collision();

void player_init(const fix16 pos_x, const fix16 pos_y)
 {
    p.pos_x = pos_x; 
    p.pos_y = pos_y; 
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
}

void update_player() 
{  
    p.pos_x = fix16Add(p.pos_x, p.dx);

    if (p.state == STATE_JUMP) 
    {
        p.dy = fix16Add(p.dy, gravity);
        check_player_floor_collision();
    }

    p.pos_y = fix16Add(p.pos_y, p.dy);
    animate_player();  
    SPR_setPosition(p.sprite, fix16ToInt(p.pos_x), fix16ToInt(p.pos_y)); 
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
                    p.dx = FIX16(0);
                    p.dy = -p.velocity;
                    p.state = STATE_WALK;
                    break;

                case DIRECTION_DOWN:
                    p.dx = FIX16(0);
                    p.dy = p.velocity;
                    p.state = STATE_WALK;
                    break;

                case DIRECTION_RIGHT:
                    p.dx = p.velocity;
                    p.dy = FIX16(0); 
                    SPR_setHFlip(p.sprite, FALSE);
                    p.state = STATE_WALK;
                    break;

                case DIRECTION_LEFT: 
                    p.dx = -p.velocity;
                    p.dy = FIX16(0);
                    SPR_setHFlip(p.sprite, TRUE);
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
                    SPR_setHFlip(p.sprite, FALSE);
                    break;

                case DIRECTION_LEFT: 
                    p.dx = -p.velocity;
                    SPR_setHFlip(p.sprite, TRUE);
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
        jumping_point = fix16ToInt(p.pos_y) + p.sprite_height; 
        p.dy = FIX16(-JUMP_VELOCITY);
    }
}

void animate_player()
{
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
    }
}

void check_player_floor_collision() 
{
    if (fix16ToInt(p.pos_y) + p.sprite_height > jumping_point) 
    {
        p.dy = FIX16(0);
        p.pos_y = FIX16(jumping_point - p.sprite_height);
        jumping_point = 0;
        p.state = STATE_STAND; 
    }
}