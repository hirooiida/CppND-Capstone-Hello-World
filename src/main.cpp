#include <iostream>
#include "game.h"

int main() {
    constexpr std::size_t kScreenWidth{1280};
    constexpr std::size_t kScreenHeight{768};

    Game game;
    bool init_result = game.Initialize(kScreenWidth, kScreenHeight);
    
    if(init_result)
    {
        game.Run();
    }
    
    game.Shutdown();

    return 0;
}