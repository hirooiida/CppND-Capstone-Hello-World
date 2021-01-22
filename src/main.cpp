#include <iostream>
#include "game.h"
#include "controller.h"
#include "renderer.h"
#include "config.h"

int main() {
    Config config{1280,    // screen width
                  768,     // screen height
                  18,      // ego thickness
                  30,      // food thickness
                  25.0f,   // wall thickness
                  100      // holl width
                  };

    Renderer renderer(config);
    Controller controller;
    Game game(config);

    game.Run(controller, renderer);

    return 0;
}