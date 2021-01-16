#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "wall.h"
#include "controller.h"
#include "vector2.h"

class Game
{
public:
    Game(const std::size_t width, const std::size_t height);

    void Run(Controller controller);
    void Shutdown();

private:
    void UpdateGame();
    void Render();

    bool is_running_;

    SDL_Window* sdl_window_;
    SDL_Renderer* sdl_renderer_;

    Uint32 last_time_;

    Vector2 ego_dir_;
    Vector2 ego_position_;

    Vector2 food_position_;

    std::vector<Wall> walls_;
};

#endif
