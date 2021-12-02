#include "play_state.h"
#include "control.h"

void play_state_init()
{
    player_init(p, 100.f, 100.f);  
}

void play_state_render()
{
    
}

void play_state_update()
{
    update_player(p);   
    SPR_update();
    SYS_doVBlankProcess();
}

void play_state_clean()
{
} 