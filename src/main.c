#include <genesis.h>
#include "resources.h"
#include "game.h"
#include "title_state.h"
#include "menu_state.h"
#include "options_state.h"
#include "play_state.h"

int main()
{ 
    game_init(); 
    while(1)
    {
        switch (current_state)
        {
            case TITLE_STATE:
                process_title_state(); 
                break;

            case MENU_STATE:
                process_menu_state();
                break;

            case PLAY_STATE:
                process_play_state();
                break;

            case OPTIONS_STATE:
                process_options_state();
                break;
        }
    }
    return 0;
}
