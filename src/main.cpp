#include <iostream>
#include "game.h"
#include "controller.h"
#include "renderer.h"

int main() {
    constexpr std::size_t kScreenWidth{1280};
    constexpr std::size_t kScreenHeight{768};

    Renderer renderer(kScreenWidth, kScreenHeight);
    Controller controller;
    Game game(kScreenWidth, kScreenHeight);

    game.Run(controller, renderer);

    return 0;
}