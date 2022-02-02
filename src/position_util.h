#ifndef _POSITION_UTIL_H_
#define _POSITION_UTIL_H_

#include <genesis.h>

fix16 to_game_pos_x(const fix16 pos_x, uint16_t sprite_width);
fix16 to_game_pos_y(const fix16 pos_y, uint16_t sprite_height);
fix16 to_sprite_pos_x(const fix16 pos_x, uint16_t sprite_width);
fix16 to_sprite_pos_y(const fix16 pos_y, uint16_t sprite_height);

#endif