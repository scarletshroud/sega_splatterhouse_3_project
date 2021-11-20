#ifndef _GAME_H_
#define _GAME_H_

enum GAME_STATE 
{
    TITLE_STATE,
    MENU_STATE, 
    PLAY_STATE,
    OPTIONS_STATE, 
};

enum GAME_STATE currentState; 

void gameInit(); 

void processTitleState(); 
void processMenuState(); 
void processPlayState(); 
void processOptionsState();

#endif