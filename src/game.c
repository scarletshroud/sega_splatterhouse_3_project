#include "game.h"
#include "control.h"

#include "title_state.h"
#include "menu_state.h"
#include "play_state.h"
#include "options_state.h"
#include "sgdk_redefinitions.h"

void game_init() {
    current_state = PLAY_STATE;
    JOY_init();    
    SPR_init();
}

void process_title_state() {
    JOY_setEventHandler(&title_control_handler); 

    title_state_init();
    title_state_render(); 

    while (current_state == TITLE_STATE) {
        title_state_update(); 
    }

    title_state_clean(); 
}

void process_menu_state() {
    JOY_setEventHandler(&menu_control_handler);

    menu_state_init();
    menu_state_render();
    
    while (current_state == MENU_STATE) {
        menu_state_update(); 
    }

    menu_state_clean(); 
}

void process_play_state() {
    JOY_setEventHandler(&play_control_handler);

    play_state_init();
    play_state_render();

    while (current_state == PLAY_STATE) {
        play_state_update();
        VDP_showFPS(FALSE); 
    }

    play_state_clean(); 
}

void process_options_state() {
    JOY_setEventHandler(&options_control_handler);

    options_state_init();
    options_state_render();
    
    while (current_state == OPTIONS_STATE) {
        options_state_update(); 
    }

    options_state_clean(); 
}

void set_game_state(enum GAME_STATE state) {
    current_state = state;
}