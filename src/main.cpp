#include <iostream>
#include "game.h"
#include "controller.h"

int main() {
    constexpr std::size_t kScreenWidth{1280};
    constexpr std::size_t kScreenHeight{768};

    Game game(kScreenWidth, kScreenHeight);
    Controller controller;
    
    game.Run(controller);
    
    game.Shutdown();

    return 0;
}