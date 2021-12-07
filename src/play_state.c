#include "play_state.h"
#include "control.h"
#include "resources.h"

void play_state_init()
{
    VDP_setPalette(PAL1, rick.palette->data);
    player_init(FIX16(100), FIX16(100));  
}

void play_state_render()
{
    
}

void play_state_update()
{ 
    update_player();   
    SPR_update();
    SYS_doVBlankProcess();
}

void play_state_clean()
{
} 