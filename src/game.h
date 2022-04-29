#ifndef _GAME_H_
#define _GAME_H_

enum GAME_STATE {
    TITLE_STATE,
    MENU_STATE, 
    PLAY_STATE,
    OPTIONS_STATE,
    GAME_OVER_STATE
};

enum GAME_STATE current_state; 

void game_init(); 
void process_title_state(); 
void process_menu_state(); 
void process_play_state(); 
void process_options_state();
void process_game_over_state();
void set_game_state(enum GAME_STATE state);

#endif