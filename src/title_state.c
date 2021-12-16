#include "title_state.h"
#include "control.h"
#include "resources.h"

#define PLANE_WIDTH 19
#define PLANE_HEIGHT 127

#define HORIZONTAL_SCROLL_VELOCITY 0.3
#define VERTICAL_SCROLL_VELOCITY 0.4

static short offset_x;
static short offset_y; 

static unsigned short timer;

static void render_title_text();
static void render_jennifer_animation();  

void title_state_init()
{
    VDP_setPlaneSize(PLANE_WIDTH, PLANE_HEIGHT, TRUE);
    VDP_setTextPlane(BG_A);
    VDP_loadTileSet(title_background.tileset, 1, DMA);
    VDP_setPalette(PAL1, title_background.palette->data);
    VDP_setTileMapEx(BG_B, title_background.tilemap, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 1), 0, 3, 0, 0, PLANE_HEIGHT, PLANE_WIDTH, DMA);
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE); 

    offset_x = FIX16(0);
    offset_y = FIX16(0);

    timer = 0; 

    XGM_setLoopNumber(-1);
    XGM_startPlay(title_track);
}

void title_state_render()
{
    render_title_text();
}

void title_state_update() 
{
    timer++;
    
    if (timer < 1300) {
        offset_x = fix16Sub(offset_x, FIX16(HORIZONTAL_SCROLL_VELOCITY));
        offset_y = fix16Add(offset_y, FIX16(VERTICAL_SCROLL_VELOCITY));

        VDP_setHorizontalScroll(BG_B, fix16ToInt(offset_x));
        VDP_setVerticalScroll(BG_A, fix16ToInt(offset_y));
    } else {
        VDP_resetScreen();
        render_jennifer_animation();
    }

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

    VDP_drawText("IT WAS A TERRIBLE NIGHT",             text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("THEY CAME BACK",                      text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("TO MY HOME",                          text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("GOD... NO..",                         text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("THEY TOOK AWAY MY SON -",             text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("DAVID AND JENNIFER",                  text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("THEY WANT TO PERFORM",                text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("A BRUTAL RITUAL",                     text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("ON DAVID.",                           text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("I MUSTN'T LET THIS",                  text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("HAPPEN..",                            text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("I FOUND THIS DAMN MASK",              text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("THESE CREATURES",                     text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("MUST BE",                             text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("ON THE FIRST FLOOR OF",               text_pos_x, text_pos_y + (text_offset++));
    VDP_drawText("MY FUCKING HOME..",                   text_pos_x, text_pos_y + (text_offset++));
}

void render_jennifer_animation() {
    unsigned short palette_full[64];
    memcpy(&palette_full[0], jennifer_image.palette->data, 16 * 2);
    PAL_setPaletteColorsDMA(0, palette_black);
	
    VDP_drawImage(BG_B, &jennifer_image, 10, 5);
	
    PAL_fadeIn(0, 40, palette_full, 100, FALSE);
    waitMs(3000);
    PAL_fadeOut(0, 40, 100, FALSE);

    while(1) {
        VDP_waitVSync();
    }
}