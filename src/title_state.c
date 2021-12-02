#include "title_state.h"
#include "control.h"
#include "resources.h"

void render_title_text(); 

void title_state_init()
{
    VDP_setPlaneSize(19, 127, TRUE);
    VDP_setTextPlane(BG_A);
    VDP_loadTileSet(title_background.tileset, 1, DMA);
    VDP_setPalette(PAL1, title_background.palette->data);
    VDP_setTileMapEx(BG_B, title_background.tilemap, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 1), 0, 3, 0, 0, 127, 19, DMA);
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    offset_x = FIX16(0);
    offset_y = FIX16(0);
    XGM_setLoopNumber(-1);
    XGM_startPlay(title_track);
}

void title_state_render()
{
    render_title_text();
}

void title_state_update() 
{
    offset_y = fix16Add(offset_y, FIX16(0.2));
    offset_x = fix16Sub(offset_x, FIX16(0.3));
    VDP_setHorizontalScroll(BG_B, fix16ToInt(offset_x));
    VDP_setVerticalScroll(BG_A, fix16ToInt(offset_y));
    SYS_doVBlankProcess();
} 

void title_state_clean()
{
    XGM_stopPlay();
} 

void render_title_text() 
{
    short text_pos_x = 9; 
    short text_pos_y = 30; 
    short text_offset = 0;

    VDP_drawText("THIS CITY WAS ONCE A", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("MOST EXCELLENT", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("PLACE... UNTIL ONE", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("DAY, A POWERFUL BUNCH", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("OF BAD DUDES", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("TOOK OVER.", text_pos_x, text_pos_y + (text_offset++));

    VDP_drawText("THESE HEINOUS DUDES", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("SOON HAD CONTROL OF THE", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("GOVERNMENT AND EVEN", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("THE POLICE FORCE. THE", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("CITY HAS BECOME A", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("TYPICAL SETTING FOR", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("A BEAT 'EM UP.", text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("BOGUS.", text_pos_x, text_pos_y + (text_offset++));
}