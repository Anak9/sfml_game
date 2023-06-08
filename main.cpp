#include<cstdlib>

#include "Game.h"

int main()
{
    // initialize rand
    srand(static_cast<unsigned>(time(0)));

    Game game;
    game.run();

    // end of application
    return 0;
}


