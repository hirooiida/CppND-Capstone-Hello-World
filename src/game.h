#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "wall.h"
#include "vector2.h"
#include "controller.h"
#include "renderer.h"
#include "config.h"

class Game
{
public:
    Game(Config config);

    void Run(Controller controller, Renderer renderer);

private:
    void UpdateGame();

    bool is_running_;
    Uint32 last_time_;

    Vector2 ego_dir_;
    Vector2 ego_position_;
    Vector2 food_position_;

    std::vector<Wall> walls_;
    Config config_;
};

#endif
