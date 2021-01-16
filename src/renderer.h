#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "SDL.h"
#include "wall.h"
#include "vector2.h"

class Renderer
{
public:
    Renderer(const std::size_t width, const std::size_t height);
    ~Renderer();
    void Render(Vector2 ego_position, std::vector<Wall> walls, Vector2 food_position);
    SDL_Window* GetWindow();

private:
    SDL_Window* sdl_window_;
    SDL_Renderer* sdl_renderer_;
};

#endif
