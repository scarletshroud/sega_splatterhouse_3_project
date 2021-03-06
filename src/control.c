#include <genesis.h>

#include "game.h"
#include "zombie_list.h"
#include "player.h"
#include "control.h"
#include "menu_state.h"
#include "play_state.h"

void title_control_handler(unsigned short joy, unsigned short changed, unsigned short state) {
    if (joy == JOY_1) {
        if (changed & state & BUTTON_START) {
            set_game_state(MENU_STATE);
        }
    }
}

void play_control_handler(unsigned short joy, unsigned short changed, unsigned short state) {
    if (joy == JOY_1) {

        if (state & BUTTON_LEFT) {
            player_set_direction(DIRECTION_LEFT);
        } else if (state & BUTTON_RIGHT) {
            player_set_direction(DIRECTION_RIGHT);
        } else if (state & BUTTON_UP) {
            player_set_direction(DIRECTION_UP);
        } else if (state & BUTTON_DOWN) {
            player_set_direction(DIRECTION_DOWN);
        } else if ((changed & (BUTTON_RIGHT | BUTTON_LEFT | BUTTON_UP | BUTTON_DOWN))) {
            player_set_direction(DIRECTION_NONE); 
        }

        if (state & BUTTON_C) {
            player_set_state(STATE_JUMP); 
        }

        if (state & BUTTON_A) {
            player_set_state(STATE_FIRST_HIT);
        }

        if (state & BUTTON_B) {
            player_set_state(STATE_BEAST_TRANSORMATION);
        }   
    }
}

void menu_control_handler(unsigned short joy, unsigned short changed, unsigned short state) {
    if (joy == JOY_1) {

        if (changed & state & BUTTON_UP) {
            move_cursor_up(); 
        }

        if (changed & state & BUTTON_DOWN) {
            move_cursor_down(); 
        }

        if (changed & state & BUTTON_START) {
            switch (current_menu_option) { 
                case MENU_OPTION_START:
                    set_game_state(PLAY_STATE);
                    break;
                case MENU_OPTION_OPTIONS:
                    break;
            }        
        }
    }
}

void options_control_handler(unsigned short joy, unsigned short changed, unsigned short state) {
}

void game_over_control_handler(unsigned short joy, unsigned short changed, unsigned short state) {
    if (joy == JOY_1) {
        if (changed & state & BUTTON_START) {
            set_game_state(TITLE_STATE);       
        }
    }
}

