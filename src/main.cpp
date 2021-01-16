#include <iostream>
#include "game.h"
#include "controller.h"

int main() {
    constexpr std::size_t kScreenWidth{1280};
    constexpr std::size_t kScreenHeight{768};

    Game game;
    Controller controller;
    bool init_result = game.Initialize(kScreenWidth, kScreenHeight);
    
    if(init_result)
    {
        game.Run(controller);
    }
    
    game.Shutdown();

    return 0;
}