#ifndef SNAKE_H
#define SNAKE_H

#include <SDL.h>


struct Vector2
{
    float x;
    float y;
};

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

    Uint32 last_time_;

    Vector2 ego_dir_;
    Vector2 ego_position_;
};

#endif
