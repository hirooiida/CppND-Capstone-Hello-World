#include <iostream>
#include "game.h"
#include "controller.h"
#include "renderer.h"
#include "config.h"

int main() {
    Config config{1000,    // screen width
                  600,     // screen height
                  20,      // ego thickness
                  32,      // food thickness
                  30.0f,   // wall thickness
                  320.0f   // holl width
                  };

    Renderer renderer(config);
    Controller controller;
    Game game(config);

    game.Run(controller, renderer);

    std::cout << "Food Score: " << game.GetFoodScore() << std::endl;
    std::cout << "Wall Score: " << game.GetWallScore() << std::endl;
    std::cout << "Total Score: " << game.GetTotalScore() << std::endl;

    return 0;
}