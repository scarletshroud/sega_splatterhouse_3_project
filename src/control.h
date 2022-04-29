#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <genesis.h>

void title_control_handler(unsigned short joy, unsigned short changed, unsigned short state);
void play_control_handler(unsigned short joy, unsigned short changed, unsigned short state); 
void menu_control_handler(unsigned short joy, unsigned short changed, unsigned short state);
void options_control_handler(unsigned short joy, unsigned short changed, unsigned short state); 
void game_over_control_handler(unsigned short joy, unsigned short changed, unsigned short state);

#endif