#include "position_util.h"

fix16 to_game_pos_x(const fix16 pos_x, uint16_t sprite_width) {
    return fix16_add(pos_x, FIX16(sprite_width / 2));  
}

fix16 to_game_pos_y(const fix16 pos_y, uint16_t sprite_height) {
    return fix16_add(pos_y, FIX16(sprite_height / 2));  
}

fix16 to_sprite_pos_x(const fix16 pos_x, uint16_t sprite_width) {
    return fix16_sub(pos_x, FIX16(sprite_width / 2)); 
}

fix16 to_sprite_pos_y(const fix16 pos_y, uint16_t sprite_height) {
    return fix16_sub(pos_y, FIX16(sprite_height / 2));
}