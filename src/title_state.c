#include "headers/title_state.h"
#include "headers/control.h"

void titleStateInit()
{
    JOY_setEventHandler(&titleControlHandler);
}

void titleStateRender()
{

}

void titleStateUpdate() 
{
    SPR_update();
    SYS_doVBlankProcess();
} 

void titleStateClean()
{

} 