#include "control.h"

#include <genesis.h>

#include "game.h"
#include "player.h"
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
        if (changed & state & BUTTON_LEFT) {
           player_move_left(p);
        }

        if (changed & state & BUTTON_RIGHT) {
           player_move_right(p);
        }

        if (changed & state & BUTTON_UP) {
           player_move_up(p);
        }

        if (changed & state & BUTTON_DOWN) {
           player_move_down(p);
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
                case MENU_OPTION_EXIT:
                    break;
            }        
        }
    }
}

void options_control_handler(unsigned short joy, unsigned short changed, unsigned short state) {

}
