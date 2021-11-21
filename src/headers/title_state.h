#ifndef _TITLE_STATE_H_
#define _TITLE_STATE_H_

#include <genesis.h>

static const short welcomeTextPosX = 9; 
static const short welcomeTextPosY = 30; 
static fix16 offsetX;
static fix16 offsetY; 

void titleStateInit(); 
void tiltleStateRender(); 
void titleStateUpdate(); 
void titleStateClean(); 

#endif