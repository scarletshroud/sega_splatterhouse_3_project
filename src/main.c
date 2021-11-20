#include <genesis.h>
#include "resources.h"
#include "headers/game.h"

int main()
{ 
    gameInit(); 
    while(1)
    {
        switch (currentState)
        {
            case TITLE_STATE:
                processTitleState(); 
                break;

            case MENU_STATE:
                processMenuState();
                break;

            case PLAY_STATE:
                processPlayState();
                break;

            case OPTIONS_STATE:
                processOptionsState;
                break;
        }
    }
    return 0;
}
