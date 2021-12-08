#include "play_state.h"

#include "camera.h"
#include "player.h"
#include "control.h"
#include "resources.h"

Map *map;
static struct camera cam; 

void play_state_init()
{
    short index = TILE_USERINDEX;
    VDP_loadTileSet(&level_tileset, index, DMA);
    VDP_setPaletteColors(PAL0, level_palette.data, 16 * 2);
    map = MAP_create(&level_map, BG_A, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX));
    VDP_setPalette(PAL1, rick.palette->data);
    VDP_setPalette(PAL2, zombie_sprite.palette->data);
    player_init(FIX16(100), FIX16(100));  
    camera_init(&cam, map, 0, 0);
    zombie_init(&z, FIX16(200), FIX16(80));
}

void play_state_render()
{
}

void play_state_update()
{ 
    update_player();
    zombie_update(&z);
    struct player_position p_pos = get_player_position();
    move_camera_horizontally(&cam, p_pos.x);   
    SPR_update();
    SYS_doVBlankProcess();
}

void play_state_clean()
{
} 