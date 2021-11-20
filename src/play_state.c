#include "headers/play_state.h"
#include "headers/control.h"

void playStateInit()
{
    JOY_setEventHandler(&playControlHandler);
    SPR_init(0, 0, 0);
    playerInit(p, 100.f, 100.f);  
}

void playStateRender()
{

}

void playStateUpdate()
{
    playerMove(p);    
    SPR_update();
    SYS_doVBlankProcess();
}

void playStateClean()
{
} 