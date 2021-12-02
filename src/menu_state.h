#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include <genesis.h>

enum MENU_OPTION {
    MENU_OPTION_START,
    MENU_OPTION_OPTIONS,
    MENU_OPTION_EXIT
};

enum MENU_OPTION current_menu_option; 

void menu_state_init(); 
void menu_state_render(); 
void menu_state_update(); 
void menu_state_clean(); 

void move_cursor_down();
void move_cursor_up(); 

#endif