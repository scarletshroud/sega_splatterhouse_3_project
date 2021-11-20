#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <genesis.h>

void titleControlHandler(u16 joy, u16 changed, u16 state);
void playControlHandler(u16 joy, u16 changed, u16 state); 
void menuControlHandler(u16 joy, u16 changed, u16 state);
void optionsControlHandler(u16 joy, u16 changed, u16 state); 

#endif