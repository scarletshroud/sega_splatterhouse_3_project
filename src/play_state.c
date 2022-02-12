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
    short index = TILE_USERINDEX;

    VDP_loadTileSet(&level_tileset, index, DMA);
    VDP_setPaletteColors(PAL0, level_palette.data, 16 * 2);

    map = MAP_create(&level_map, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, index));

    VDP_setPalette(PAL1, rick.palette->data);
    VDP_setPalette(PAL2, zombie_sprite.palette->data);
    VDP_setTextPlane(BG_A);

    player_init(FIX16(START_PLAYER_POS_X), FIX16(START_PLAYER_POS_Y));  
    camera_init(&cam, map, FIX16(0), FIX16(0), FIX16(2));

    //zombie_list_push_back(&head, create_zombie(FIX16(100), FIX16(100)));
    //zombie_list_push_back(&head, create_zombie(FIX16(150), FIX16(150))); 
} 

void play_state_render() {

}

static void all_zombie_update() {
    struct zombie_list* current = head;
    while (current != NULL) {
        zombie_update(current->z);
        current = current->next; 
    }
}

void play_state_update() { 
    player_update();
    all_zombie_update(); 
    camera_update(&cam); 

    clean_zombie();

    SPR_update();
    SYS_doVBlankProcess();
}

static void clean_zombie() {
    struct zombie_list* current = head;
    while (current != NULL && current->z->state == ZOMBIE_STATE_NONE) {
        struct zombie_list* prev = current; 
        current = current->next; 
        zombie_list_remove(head, prev); 
    }
}

void play_state_clean() {
} 