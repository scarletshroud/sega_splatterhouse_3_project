#include "play_state.h"

#include "camera.h"
#include "zombie.h"
#include "player.h"
#include "control.h"
#include "resources.h"

Map *map;
static struct camera cam; 

static void clean_zombie();

#define START_PLAYER_POS_X 100
#define START_PLAYER_POS_Y 100
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
    zombies[1] = create_zombie(FIX16(100), FIX16(100));
    //zombie_list_push_back(&head, create_zombie(FIX16(50), FIX16(50)));
    //zombie_list_push_back(&head, create_zombie(FIX16(150), FIX16(150))); 
} 

#define HUD_TEXT_TILE_Y 25
#define LIFES_TEXT_TILE_X 30 
#define HEALTH_TEXT_TILE_X 1
#define ENERGY_TEXT_TILE_X 15 
static void draw_hud() {
    VDP_drawText("Health:",  HEALTH_TEXT_TILE_X, HUD_TEXT_TILE_Y);
    VDP_drawText("Energy:",  ENERGY_TEXT_TILE_X, HUD_TEXT_TILE_Y);
    VDP_drawText("Lifes:",  LIFES_TEXT_TILE_X, HUD_TEXT_TILE_Y);
}

#define VALUE_OFFSET 7
#define VALUE_MAX_LENGTH 3
static void draw_hud_values() {
    char health[VALUE_MAX_LENGTH];
    char energy[VALUE_MAX_LENGTH];
    char lifes[VALUE_MAX_LENGTH];

    sprintf(health, "%d", get_player_health());
    sprintf(energy, "%d", get_player_energy());
    sprintf(lifes, "%d", get_player_lifes());

    VDP_drawText(health,  HEALTH_TEXT_TILE_X + VALUE_OFFSET, HUD_TEXT_TILE_Y);
    VDP_drawText(energy,  ENERGY_TEXT_TILE_X + VALUE_OFFSET, HUD_TEXT_TILE_Y);
    VDP_drawText(lifes,  LIFES_TEXT_TILE_X + VALUE_OFFSET, HUD_TEXT_TILE_Y);
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

    for (uint8_t i = 0; i < 2; ++i) {
        //KLog_U1("state ", zombies[i]->health);
        if (zombies[i]->state != ZOMBIE_STATE_NONE)
            zombie_update(zombies[i]);
        else 
            SPR_releaseSprite(zombies[i]->sprite);
    }
}

void play_state_update() { 
    player_update();
    all_zombie_update(); 
    camera_update(&cam); 

    //KLog_U1("state ", p.state);
    /*zombie_print_list(head); */
    
    clear_hud_values();
    draw_hud_values();

    clean_zombie();
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
} 