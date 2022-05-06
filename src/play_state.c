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

#define START_PLAYER_POS_X 0
#define START_PLAYER_POS_Y 0
#define REMAINING_TIME 60
void play_state_init() {
    short index = 1;

    VDP_loadTileSet(&level_tileset, index, DMA);
    VDP_setPaletteColors(PAL0, level_palette.data, 16 * 2);

    map = MAP_create(&level_map, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, index));

    VDP_setPalette(PAL1, rick.palette->data);
    VDP_setPalette(PAL2, zombie_sprite.palette->data);
    VDP_setTextPlane(BG_A);

    player_init(FIX16(START_PLAYER_POS_X), FIX16(START_PLAYER_POS_Y)); 
    camera_init(&cam, map, FIX16(0), FIX16(0), FIX16(2));

    zombies[0] = create_zombie(FIX16(50), FIX16(50));
    //zombies[1] = create_zombie(FIX16(100), FIX16(100));
    //zombie_list_push_back(&head, create_zombie(FIX16(50), FIX16(50)));
    //zombie_list_push_back(&head, create_zombie(FIX16(150), FIX16(150)));

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

#define HUD_TEXT_TILE_Y 25
#define LIFES_TEXT_TILE_X 30 
#define HEALTH_TEXT_TILE_X 1
#define ENERGY_TEXT_TILE_X 15 
#define TIMER_TEXT_TILE_X 5
#define TIMER_TEXT_TILE_Y 1  
static void draw_hud() {
    VDP_drawText("Health:",  HEALTH_TEXT_TILE_X, HUD_TEXT_TILE_Y);
    VDP_drawText("Energy:",  ENERGY_TEXT_TILE_X, HUD_TEXT_TILE_Y);
    VDP_drawText("Lifes:",   LIFES_TEXT_TILE_X,  HUD_TEXT_TILE_Y);
    VDP_drawText("Time: ",   TIMER_TEXT_TILE_X,  TIMER_TEXT_TILE_Y);
}

#define VALUE_OFFSET 7
#define VALUE_MAX_LENGTH 3
static void draw_hud_values() {
    char health[VALUE_MAX_LENGTH];
    char energy[VALUE_MAX_LENGTH];
    char lifes[VALUE_MAX_LENGTH];

    sprintf(health, "%d", player_get_health());
    sprintf(energy, "%d", player_get_energy());
    sprintf(lifes, "%d", player_get_lifes());
    

    VDP_drawText(health,  HEALTH_TEXT_TILE_X + VALUE_OFFSET, HUD_TEXT_TILE_Y);
    VDP_drawText(energy,  ENERGY_TEXT_TILE_X + VALUE_OFFSET, HUD_TEXT_TILE_Y);
    VDP_drawText(lifes,  LIFES_TEXT_TILE_X + VALUE_OFFSET, HUD_TEXT_TILE_Y);
}

static void draw_hud_time() {
    char time[VALUE_MAX_LENGTH];
    sprintf(time, "%d", calculate_ramaining_time());
    VDP_drawText(time,  TIMER_TEXT_TILE_X + VALUE_OFFSET, TIMER_TEXT_TILE_Y);
}

static void clear_hud_time() {
    VDP_clearText(TIMER_TEXT_TILE_X + VALUE_OFFSET, TIMER_TEXT_TILE_Y, VALUE_MAX_LENGTH);
}

static void clear_hud_values() {
    VDP_clearText(HEALTH_TEXT_TILE_X + VALUE_OFFSET, HUD_TEXT_TILE_Y, VALUE_MAX_LENGTH);
    VDP_clearText(ENERGY_TEXT_TILE_X + VALUE_OFFSET, HUD_TEXT_TILE_Y, VALUE_MAX_LENGTH);
    VDP_clearText(LIFES_TEXT_TILE_X + VALUE_OFFSET, HUD_TEXT_TILE_Y, VALUE_MAX_LENGTH);
}

void play_state_render() {
    draw_hud();
    draw_hud_values();
}

static void all_zombie_update() {
   /* struct zombie_list* current = head;
    while (current != NULL) {
        zombie_update(current->z);
        current = current->next; 
    } */

    for (uint8_t i = 0; i < 1; ++i) {
        //KLog_U1("z state ", zombies[i]->state);
        if (zombies[i]->state != ZOMBIE_STATE_NONE)
            zombie_update(zombies[i]);
        else 
            SPR_releaseSprite(zombies[i]->sprite);
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

    clean_zombie();
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

static void clean_zombie() {
    /*struct zombie_list* current = head;
    while (current != NULL) {

        if (current->z->state == ZOMBIE_STATE_NONE) {
            zombie_list_remove(&head, current); 
        }
 
        current = current->next;
    }*/
}

void play_state_clean() {
    clean_player();
    clear_hud_time();
    clear_hud_values();
    VDP_resetScreen();
} 