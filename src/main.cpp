#include <iostream>
#include "game.h"

int main() {
    Game game;
    bool init_result = game.Initialize();
    

    if(init_result)
    {
        game.Run();
    }
    
    game.Shutdown();

    return 0;
}