#include "play_state.h"

#include "game.h"
#include "camera.h"
#include "zombie.h"
#include "player.h"
#include "control.h"
#include "resources.h"
#include <timer.h>

Map *map;
static struct camera cam; 

static void clean_zombie();
static uint32_t start_time;

#define PLANE_WIDTH 64
#define PLANE_HEIGHT 32

#define START_PLAYER_POS_X 0
#define START_PLAYER_POS_Y 0
#define REMAINING_TIME 60
void play_state_init() {
    short index = 1;
    head = NULL;

    VDP_setPlaneSize(PLANE_WIDTH, PLANE_HEIGHT, TRUE);

    VDP_loadTileSet(&level_tileset, index, DMA);
    VDP_setPaletteColors(PAL0, level_palette.data, 16 * 2);

    map = MAP_create(&level_map, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, index));

    VDP_setPalette(PAL1, rick.palette->data);
    VDP_setPalette(PAL2, zombie_sprite.palette->data);
    VDP_setTextPlane(BG_A);

    player_init(FIX16(START_PLAYER_POS_X), FIX16(START_PLAYER_POS_Y)); 
    camera_init(&cam, map, FIX16(0), FIX16(0), FIX16(2));

    zombie_list_push_back(&head, create_zombie(FIX16(50), FIX16(50)));
    zombie_list_push_back(&head, create_zombie(FIX16(150), FIX16(60)));

    XGM_setLoopNumber(-1);
    XGM_startPlay(level_track); 

    startTimer(0);
    start_time = getSubTick();
} 

static uint32_t calculate_seconds() {
    return (getSubTick() - start_time) / SUBTICKPERSECOND;
} 

static uint32_t calculate_ramaining_time() {
    return REMAINING_TIME - calculate_seconds();
}

#define BOTTOM_HUD_TEXT_TILE_Y 25
#define LIFES_TEXT_TILE_X 29 
#define HEALTH_TEXT_TILE_X 2
#define ENERGY_TEXT_TILE_X 16 
#define TIMER_TEXT_TILE_X 5
#define UPPER_HUD_TEXT_TILE_Y 1  
static void draw_hud() {
    VDP_drawText("Health:",  HEALTH_TEXT_TILE_X, BOTTOM_HUD_TEXT_TILE_Y);
    VDP_drawText("Energy:",  ENERGY_TEXT_TILE_X, BOTTOM_HUD_TEXT_TILE_Y);
    VDP_drawText("Lifes:",   LIFES_TEXT_TILE_X,  BOTTOM_HUD_TEXT_TILE_Y);
    VDP_drawText("Time: ",   TIMER_TEXT_TILE_X,  UPPER_HUD_TEXT_TILE_Y);
}

#define VALUE_OFFSET 7
#define VALUE_MAX_LENGTH 4
static void draw_hud_values() {
    char health[VALUE_MAX_LENGTH];
    char energy[VALUE_MAX_LENGTH];
    char lifes[VALUE_MAX_LENGTH];

    sprintf(energy, "%d", player_get_energy());
    sprintf(health, "%d", player_get_health());
    sprintf(lifes, "%d", player_get_lifes());
    
    VDP_drawText(health,  HEALTH_TEXT_TILE_X + VALUE_OFFSET, BOTTOM_HUD_TEXT_TILE_Y);
    VDP_drawText(energy,  ENERGY_TEXT_TILE_X + VALUE_OFFSET, BOTTOM_HUD_TEXT_TILE_Y);
    VDP_drawText(lifes,  LIFES_TEXT_TILE_X + VALUE_OFFSET, BOTTOM_HUD_TEXT_TILE_Y);
}

static void draw_hud_time() {
    char time[VALUE_MAX_LENGTH];
    sprintf(time, "%d", calculate_ramaining_time());
    VDP_drawText(time,  TIMER_TEXT_TILE_X + VALUE_OFFSET, UPPER_HUD_TEXT_TILE_Y);
}

static void clear_hud_time() {
    VDP_clearText(TIMER_TEXT_TILE_X + VALUE_OFFSET, UPPER_HUD_TEXT_TILE_Y, VALUE_MAX_LENGTH);
}

static void clear_hud_values() {
    VDP_clearText(HEALTH_TEXT_TILE_X + VALUE_OFFSET, BOTTOM_HUD_TEXT_TILE_Y, VALUE_MAX_LENGTH);
    VDP_clearText(ENERGY_TEXT_TILE_X + VALUE_OFFSET, BOTTOM_HUD_TEXT_TILE_Y, VALUE_MAX_LENGTH);
    VDP_clearText(LIFES_TEXT_TILE_X + VALUE_OFFSET, BOTTOM_HUD_TEXT_TILE_Y, VALUE_MAX_LENGTH);
}

void play_state_render() {
    draw_hud();
    draw_hud_values();
}

static void clean_zombie(struct zombie_list* current) {
    SPR_releaseSprite(current->z->sprite);
    player_energy_up();
    zombie_list_remove(&head, current->z);
}

static void all_zombie_update() {
    struct zombie_list* current = head;
    while (current != NULL) {
        if (current->z->state != ZOMBIE_STATE_DEAD) {
            zombie_update(current->z);
        } else {
            clean_zombie(current);
            KLog_U1("list size 2 ", zombie_list_size(head));
        }
        current = current->next; 
    }
}

static void clean_player() {
    SPR_releaseSprite(player_get_sprite()); 
}

static void game_over() {
    set_game_state(GAME_OVER_STATE);
}

static bool is_player_alive() {
    return player_get_state() != STATE_DEAD;
}

static void check_remaining_time() {
    if (calculate_ramaining_time() == 0) {
        game_over(); 
    }
}

void play_state_update() { 
    player_update();

    if (!is_player_alive()) {
        game_over(); 
    }

    all_zombie_update();
    camera_update(&cam); 

    if (calculate_seconds() > 0) {
        check_remaining_time();
        clear_hud_time();
        draw_hud_time();
        startTimer(0);
        start_time = getTick();
    }

    clear_hud_values();
    draw_hud_values();

    SPR_update();
    SYS_doVBlankProcess();
}

void play_state_clean() {
    clean_player();
    clear_hud_time();
    clear_hud_values();
    VDP_resetScreen();
} 