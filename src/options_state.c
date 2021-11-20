#include "headers/options_state.h"
#include "headers/control.h"

void optionsStateInit()
{
    JOY_setEventHandler(&optionsControlHandler);
} 

void optionsStateRender() 
{

} 

void optionsStateUpdate()
{
    SPR_update();
    SYS_doVBlankProcess();
}

void optionsStateClean()
{

} 