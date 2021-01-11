#include <iostream>
#include "game.h"

int main() {
    constexpr std::size_t kScreenWidth{800};
    constexpr std::size_t kScreenHeight{800};

    Game game;
    bool init_result = game.Initialize(kScreenWidth, kScreenHeight);
    
    if(init_result)
    {
        game.Run();
    }
    
    game.Shutdown();

    return 0;
}