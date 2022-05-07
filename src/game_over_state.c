#include <genesis.h>

#include "game_over_state.h"
#include "resources.h"
#include "timer.h"
#include "game.h"

struct Sprite *jennifer_sprite;

//static struct timer frame_timer;
uint16_t timer = 0;

#define JENNIFER_POS_X 70
#define JENNIFER_POS_Y 30
void game_over_state_init() {
    VDP_setTextPlane(BG_A);
    VDP_setPalette(PAL2, jennifer_cries.palette->data);
    jennifer_sprite = SPR_addSprite(&jennifer_cries, JENNIFER_POS_X, JENNIFER_POS_Y, TILE_ATTR(PAL2, 0, FALSE, FALSE));
    XGM_startPlay(game_over_track);
    //timer_reset(&frame_timer); 
}

#define TEXT_POS_X 18
#define TEXT_POS_Y 17
void game_over_state_render() {
    VDP_drawText("Dead...", TEXT_POS_X, TEXT_POS_Y);
}

#define GAME_OVER_TIME 350
void game_over_state_update() {
    
    //timer_tick(&frame_timer);
    timer++;

    if (timer == GAME_OVER_TIME) {
        set_game_state(TITLE_STATE);
    }

    SPR_update();
    SYS_doVBlankProcess();
} 

void game_over_state_clean() {
    XGM_stopPlay();
    SPR_releaseSprite(jennifer_sprite); 
    VDP_resetScreen();
}