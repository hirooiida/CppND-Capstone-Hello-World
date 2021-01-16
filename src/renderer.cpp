#include <iostream>
#include "renderer.h"

const int ego_thickness = 15;
const int food_thickness = 30;
const int holl_width = 100;

Renderer::Renderer(const std::size_t width, const std::size_t height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    sdl_window_ = SDL_CreateWindow("Fugitive", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, width, height, 0);

    if (nullptr == sdl_window_) {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    }

    sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == sdl_renderer_) {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(sdl_renderer_);
	SDL_DestroyWindow(sdl_window_);
	SDL_Quit();
}

void Renderer::Render(Vector2 ego_position, std::vector<Wall> walls, Vector2 food_position)
{
    int w, h;
    SDL_GetWindowSize(sdl_window_, &w, &h);

    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(sdl_renderer_);

    SDL_SetRenderDrawColor(sdl_renderer_, 255, 255, 255, 255);
    SDL_Rect ball{	
		static_cast<int>(ego_position.x - ego_thickness / 2),
		static_cast<int>(ego_position.y - ego_thickness / 2),
		ego_thickness,
		ego_thickness
	};
	SDL_RenderFillRect(sdl_renderer_, &ball);

    for (Wall &wall: walls)
    {
        SDL_SetRenderDrawColor(sdl_renderer_, 255, 255, 255, 255);
        SDL_Rect horiz_wall{
            static_cast<int>(wall.x_pos - wall.thickness / 2),
            static_cast<int>(0),
            12,
            h
        };
        SDL_RenderFillRect(sdl_renderer_, &horiz_wall);

        SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x00, 0x00, 0xFF);
        SDL_Rect horiz_holl{
            static_cast<int>(wall.x_pos - wall.thickness / 2),
            static_cast<int>(wall.holl_height),
            static_cast<int>(wall.thickness),
            static_cast<int>(wall.holl_width)
        };
        SDL_RenderFillRect(sdl_renderer_, &horiz_holl);
    }

    SDL_SetRenderDrawColor(sdl_renderer_, 255, 120, 28, 255);
    SDL_Rect food{	
		static_cast<int>(food_position.x - food_thickness / 2),
		static_cast<int>(food_position.y - food_thickness / 2),
		food_thickness,
		food_thickness
	};
	SDL_RenderFillRect(sdl_renderer_, &food);

    SDL_RenderPresent(sdl_renderer_);
}

SDL_Window* Renderer::GetWindow()
{
    return sdl_window_;
}
