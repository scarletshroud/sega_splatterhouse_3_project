#include "headers/title_state.h"
#include "headers/control.h"
#include "resources.h"

void renderTitleText(); 

void titleStateInit()
{
    JOY_setEventHandler(&titleControlHandler);
    VDP_setPlaneSize(19, 127, TRUE);
    VDP_setTextPlane(BG_A);
    VDP_loadTileSet(title_background.tileset, 1, DMA);
    VDP_setPalette(PAL1, title_background.palette->data);
    VDP_setTileMapEx(BG_B, title_background.tilemap, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 1), 0, 3, 0, 0, 127, 19, DMA);
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    offsetX = FIX16(0);
    offsetY = FIX16(0);
    renderTitleText();
    XGM_setLoopNumber(-1);
    XGM_startPlay(&title_track);
}

void titleStateRender()
{
    
}

void titleStateUpdate() 
{
    offsetY = fix16Add(offsetY, FIX16(0.2));
    offsetX = fix16Sub(offsetX, FIX16(0.3));
    VDP_setHorizontalScroll(BG_B, fix16ToInt(offsetX));
    VDP_setVerticalScroll(BG_A, fix16ToInt(offsetY));
    SYS_doVBlankProcess();
} 

void titleStateClean()
{

} 

void renderTitleText() 
{
    short textPosX = 9; 
    short textPosY = 30; 
    short textOffset = 0;

    VDP_drawText("THIS CITY WAS ONCE A", textPosX, textPosY + (textOffset++));
    VDP_drawText("MOST EXCELLENT", textPosX, textPosY + (textOffset++));
    VDP_drawText("PLACE... UNTIL ONE", textPosX, textPosY + (textOffset++));
    VDP_drawText("DAY, A POWERFUL BUNCH", textPosX, textPosY + (textOffset++));
    VDP_drawText("OF BAD DUDES", textPosX, textPosY + (textOffset++));
    VDP_drawText("TOOK OVER.", textPosX, textPosY + (textOffset++));

    VDP_drawText("THESE HEINOUS DUDES", textPosX, textPosY + (textOffset++));
    VDP_drawText("SOON HAD CONTROL OF THE", textPosX, textPosY + (textOffset++));
    VDP_drawText("GOVERNMENT AND EVEN", textPosX, textPosY + (textOffset++));
    VDP_drawText("THE POLICE FORCE. THE", textPosX, textPosY + (textOffset++));
    VDP_drawText("CITY HAS BECOME A", textPosX, textPosY + (textOffset++));
    VDP_drawText("TYPICAL SETTING FOR", textPosX, textPosY + (textOffset++));
    VDP_drawText("A BEAT 'EM UP.", textPosX, textPosY + (textOffset++));
    VDP_drawText("BOGUS.", textPosX, textPosY + (textOffset++));
}