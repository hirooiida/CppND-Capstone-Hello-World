#include <iostream>
#include "game.h"
#include "controller.h"
#include "renderer.h"
#include "config.h"

int main() {
    Config config{1000,    // screen width
                  600,     // screen height
                  20,      // ego thickness
                  30,      // food thickness
                  25.0f,   // wall thickness
                  300.0f   // holl width
                  };

    Renderer renderer(config);
    Controller controller;
    Game game(config);

    game.Run(controller, renderer);

    std::cout << "Score: " << game.GetScore() << std::endl;

    return 0;
}