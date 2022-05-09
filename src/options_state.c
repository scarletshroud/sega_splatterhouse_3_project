#include "options_state.h"
#include "control.h"

void options_state_init() {
    JOY_setEventHandler(&options_control_handler);
} 

void options_state_render() {

} 

void options_state_update() {
    SPR_update();
    SYS_doVBlankProcess();
}

void options_state_clean() {

} 