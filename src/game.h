#ifndef SNAKE_H
#define SNAKE_H

#include <SDL.h>

class Game
{
public:
    Game();
    bool Initialize();
    void Run();
    void Shutdown();

private:
    void HandleInput();
    void UpdateGame();
    void Render();

    bool is_running_;

    SDL_Window* sdl_window_;
    SDL_Renderer* sdl_renderer_;
};

#endif
