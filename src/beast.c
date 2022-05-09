#include "beast.h"
#include "player.h"
#include "resources.h"
#include "play_state.h"

#define VELOCITY 2
#define DEFAULT_DX 0 
#define DEFAULT_DY 0 
#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 80   
#define DEFAULT_HEALTH 200
#define DEFAULT_DAMAGE 25

static struct beast b;
static uint16_t frame_timer = 0;

#define DISTANCE_TO_HIT 30
#define DISTANCE_TO_ATTACK 70

void beast_init(const fix16 start_pos_x, const fix16 start_pos_y) {
    b.pos_x = to_game_pos_x(start_pos_x, SPRITE_WIDTH); 
    b.pos_y = to_game_pos_y(start_pos_y, SPRITE_HEIGHT);
    b.dx = FIX16(DEFAULT_DX); 
    b.dy = FIX16(DEFAULT_DY);
    b.velocity = FIX16(VELOCITY); 
    b.health = DEFAULT_HEALTH;
    b.damage = DEFAULT_DAMAGE; 
    b.state = BEAST_STATE_STAND;
    b.injured = FALSE;
    b.sprite = SPR_addSprite(&beast_sprite, fix16ToInt(start_pos_x), fix16ToInt(start_pos_y), TILE_ATTR(PAL2, 0, FALSE, FALSE));
    b.width = SPRITE_WIDTH;
    b.height = SPRITE_HEIGHT;
    b.target_distance = DISTANCE_TO_ATTACK;
    b.frame_counter = 0;  

    //XGM_setPCM(ZOMBIE_DEATH_SOUND_ID, zombie_death_sound, sizeof(zombie_death_sound));
}

static int8_t calculate_distance_x_to_player(const struct position* const player_pos) {
    return fix16ToInt(b.pos_x) - fix16ToInt(player_pos->x);
}

static int8_t calculate_distance_y_to_player(const struct position* const player_pos) {
    return fix16ToInt(b.pos_y) - fix16ToInt(player_pos->y);
}

static bool check_axis_x_collision() {
    return !(fix16ToInt(b.pos_x) - b.width / 2  + fix16ToInt(b.dx) > SCREEN_LEFT_SIDE 
            && fix16ToInt(b.pos_x) + b.width / 2 + fix16ToInt(b.dx) < SCREEN_RIGHT_SIDE);
}

static bool check_axis_y_collision() {
    return !(fix16ToInt(b.pos_y) + b.height / 2  + fix16ToInt(b.dy) > FLOOR_UPPER_SIDE 
            && fix16ToInt(b.pos_y) + b.height / 2 + fix16ToInt(b.dy) < FLOOR_DOWN_SIDE);
}

static void update_position() {
    
    if (!check_axis_x_collision(b)) {
        b.pos_x = fix16Add(b.pos_x, b.dx);
    }

    if (!check_axis_y_collision(b)) {
        b.pos_y = fix16Add(b.pos_y, b.dy);
    }
}

static bool walk() {
    const struct position player_pos = player_get_position(); 
    const int8_t distance_x = calculate_distance_x_to_player(&player_pos);
    const int8_t distance_y = calculate_distance_y_to_player(&player_pos); 

    if (abs(distance_x) > b.target_distance) {
        b.dx = (distance_x > 0) ? -b.velocity : b.velocity;
    }

    if (abs(distance_y) > b.target_distance) {
        b.dy = (distance_y > 0) ? -b.velocity : b.velocity;  
    }

    return (abs(distance_x) <= b.target_distance && abs(distance_y) <= b.target_distance);
}

static bool walk_away() {
    const struct position player_pos = player_get_position(); 
    const int8_t distance_x = calculate_distance_x_to_player(&player_pos);
    const int8_t distance_y = calculate_distance_y_to_player(&player_pos); 

    if (abs(distance_x) < b.target_distance) {
        b.dx = (distance_x > 0) ? b.velocity : -b.velocity;
    }

    if (abs(distance_y) < b.target_distance) {
        b.dy = (distance_y > 0) ? b.velocity : -b.velocity;  
    }

    return (abs(distance_x) >= b.target_distance || abs(distance_y) >= b.target_distance);
}

#define INJURED_STATE_OFFSET 6
static void update_state(enum BEAST_STATE state) {
    b.state = state + b.injured * INJURED_STATE_OFFSET;
}

#define IMPACT_DISTANCE 40
static void hit() {
    if (b.state != BEAST_STATE_STAND && b.state != BEAST_STATE_INJURED_STAND && b.state != BEAST_STATE_INJURED_WALK && b.state != BEAST_STATE_WALK && b.state != BEAST_STATE_ATTACK) {
        return; 
    }

    const struct position player_pos = player_get_position();
    if (abs(fix16ToInt(b.pos_x) - fix16ToInt(player_pos.x)) <= IMPACT_DISTANCE && abs(fix16ToInt(b.pos_y) - fix16ToInt(player_pos.y) <= IMPACT_DISTANCE)) {
        update_state(BEAST_STATE_HIT);
        player_bang(b.damage);         
    }
}

static void horizontal_rotate() {
    if (b.dx != 0) {
        bool h_flip = (b.dx > FIX16(0)) ? TRUE : FALSE; 
        SPR_setHFlip(b.sprite, h_flip); 
    }
}

static void animate(enum BEAST_STATE state) {
    horizontal_rotate(b);
    SPR_setAnim(b.sprite, state);
}

#define ANIM_BANG_TIME 10
#define ANIM_HIT_TIME 10
#define ANIM_DIE_TIME 39
#define ANIM_LOSING_HEAD_TIME 29
static void handle_state() {
    switch (b.state) {
        case BEAST_STATE_STAND:
        case BEAST_STATE_WALK:
        case BEAST_STATE_INJURED_STAND:
        case BEAST_STATE_INJURED_WALK:

            if (walk(b)) {
                update_state(BEAST_STATE_STAND);
                b.dx = FIX16(DEFAULT_DX);
                b.dy = FIX16(DEFAULT_DY);
            } else {
                update_state(BEAST_STATE_WALK);
                update_position(b);
            }

            animate(b.state);
            break;

        case BEAST_STATE_BANG:
            if (b.frame_counter == ANIM_BANG_TIME) {
                update_state(BEAST_STATE_STAND);
                b.frame_counter = 0;
                break;
            }
            b.frame_counter++;
            animate(b.state);
            break;

        case BEAST_STATE_HIT:
        case BEAST_STATE_INJURED_HIT:
            if (b.frame_counter == ANIM_HIT_TIME) { 
                b.state = BEAST_STATE_WALK_AWAY;
                b.target_distance = DISTANCE_TO_ATTACK;
                b.frame_counter = 0;
                break;
            }
            b.frame_counter++; 
            animate(b.state);
            break;

        case BEAST_STATE_DIE:
            if (b.frame_counter == 0)
                //XGM_startPlayPCM(ZOMBIE_DEATH_SOUND_ID, 15, SOUND_PCM_CH2); 

            if (b.frame_counter == ANIM_DIE_TIME) {
                b.state = BEAST_STATE_DEAD;
                b.frame_counter = 0;
                break;
            }
            b.frame_counter++;
            animate(b.state);
            break;

        case BEAST_STATE_ATTACK:
            if (walk(b)) {
                b.dx = FIX16(DEFAULT_DX);
                b.dy = FIX16(DEFAULT_DY);
                hit(b);
            } else {
                update_position(b);
            }
            animate(BEAST_STATE_WALK + INJURED_STATE_OFFSET * b.injured);
            break;

        case BEAST_STATE_WALK_AWAY: 
            if (walk_away(b)) {
                b.state = BEAST_STATE_STAND;
                b.dx = FIX16(DEFAULT_DX);
                b.dy = FIX16(DEFAULT_DY);
            } else {
                b.state = BEAST_STATE_WALK_AWAY;
                update_position(b);        
            }
            animate(BEAST_STATE_WALK + INJURED_STATE_OFFSET * b.injured);
            break;

        case BEAST_STATE_LOSING_HEAD:
              if (b.frame_counter == ANIM_LOSING_HEAD_TIME) {
                update_state(BEAST_STATE_STAND);
                b.frame_counter = 0;
                break;
            }
            b.frame_counter++;

            animate(b.state);
            break;


        case ZOMBIE_STATE_DEAD:
            break;
    }
}

static void attack() {
    if (frame_timer == 200 && (b.state == BEAST_STATE_STAND || b.state == BEAST_STATE_WALK || b.state == BEAST_STATE_INJURED_STAND || b.state == BEAST_STATE_INJURED_WALK)) {
        b.state = BEAST_STATE_ATTACK;
        b.target_distance = DISTANCE_TO_HIT;
        frame_timer = 0;
    }

    frame_timer++;
}

void beast_update() {
    attack(b);
    handle_state(b);
    KLog_U1("state ", b.state);
    SPR_setPosition(b.sprite, fix16ToInt(to_sprite_pos_x(b.pos_x, SPRITE_WIDTH)), fix16ToInt(to_sprite_pos_y(b.pos_y, SPRITE_HEIGHT)));
}

void bang_beast(const int8_t damage) {
    if (b.state != BEAST_STATE_BANG && b.state != BEAST_STATE_INJURED_BANG) {
        if (b.health <= 0) {
            b.state = BEAST_STATE_DIE;
        } else {
            update_state(BEAST_STATE_BANG);
            b.health = b.health - damage;
            if (b.health < 100 && b.injured == FALSE) {
                b.state = BEAST_STATE_LOSING_HEAD;
                b.injured = TRUE;
            }
        }
    }
} 

struct position beast_get_position() {
    return (struct position) {.x = b.pos_x, .y = b.pos_y};
}

fix16 beast_height() {
    return b.height;
}

fix16 beast_width() {
    return b.width;
}