#include "headers/menu_state.h"
#include "headers/control.h"

void menuStateInit()
{
    JOY_setEventHandler(&menuControlHandler); 
} 

void menuStateRender()
{

} 

void menuStateUpdate()
{
    SPR_update();
    SYS_doVBlankProcess();
}

void menuStateClean()
{

} 