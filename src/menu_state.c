#include "menu_state.h"
#include "control.h"
#include "resources.h"

#define OPTIONS_NUM 2

#define LABEL_SIZE 10
typedef struct {
    uint8_t x; 
    uint8_t y; 
    char label[LABEL_SIZE]; 
} option;

static const option options[OPTIONS_NUM] = {
    {17, 11, "START"},
    {16, 12, "OPTIONS"},
};

static Sprite* cursor;

void menu_state_init() {
    current_menu_option = MENU_OPTION_START;
    cursor = SPR_addSprite(&menu_cursor, 0, 0, 0); 
    update_cursor_pos(); 
} 

void menu_state_render() {
    for (uint8_t i; i < OPTIONS_NUM; ++i) {
        VDP_drawText(options[i].label, options[i].x, options[i].y); 
    }
} 

void menu_state_update() {
    SPR_update();
    SYS_doVBlankProcess();
}

void menu_state_clean() {
    SPR_releaseSprite(&menu_cursor); 
    VDP_resetScreen();
} 

void update_cursor_pos() {
    const uint8_t tile_size = 8;
    const uint8_t cursor_offset_x = 15;  
    SPR_setPosition(cursor, options[current_menu_option].x * tile_size - cursor_offset_x, options[current_menu_option].y * tile_size); 
}

void move_cursor_up() {
    if (current_menu_option > 0) {
        current_menu_option--;
    } else {
        current_menu_option = OPTIONS_NUM - 1; 
    }
    update_cursor_pos(); 
}

void move_cursor_down() {
    if (current_menu_option < OPTIONS_NUM - 1) {
        current_menu_option++;
    } else {
        current_menu_option = 0; 
    }
    update_cursor_pos(); 
}