#include "headers/game.h"
#include "headers/control.h"

#include "headers/title_state.h"
#include "headers/menu_state.h"
#include "headers/play_state.h"
#include "headers/options_state.h"

void gameInit() 
{
    currentState = TITLE_STATE; 
}

void processTitleState() 
{
    titleStateInit();
    
    while (currentState == TITLE_STATE)
    {
        titleStateUpdate(); 
    }

    titleStateClean(); 
}

void processMenuState() 
{
    menuStateInit();
    menuStateRender();
    
    while (currentState == MENU_STATE)
    {
        menuStateUpdate(); 
    }

    menuStateClean(); 
}

void processPlayState() 
{
    playStateInit();
    playStateRender();

    while (currentState == PLAY_STATE)
    {
        playStateUpdate(); 
    }

    playStateClean(); 
}

void processOptionsState() 
{
    optionsStateInit();
    optionsStateRender();
    
    while (currentState == OPTIONS_STATE)
    {
        optionsStateUpdate(); 
    }

    optionsStateClean(); 
}