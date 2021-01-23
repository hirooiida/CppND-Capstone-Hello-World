#include <iostream>
#include "renderer.h"

Renderer::Renderer(Config config)
{
    config_ = config;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    sdl_window_ = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, config_.screen_width, config_.screen_height, 0);

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
    SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, 255);
    SDL_RenderClear(sdl_renderer_);

    // Wall
    for (Wall &wall: walls)
    {
        SDL_SetRenderDrawColor(sdl_renderer_, 255, 255, 255, 255);
        SDL_Rect horiz_wall{
            static_cast<int>(wall.x_pos - wall.thickness / 2),
            static_cast<int>(0),
            static_cast<int>(wall.thickness),
            static_cast<int>(config_.screen_height)
        };
        SDL_RenderFillRect(sdl_renderer_, &horiz_wall);

        SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, 255);
        SDL_Rect horiz_holl{
            static_cast<int>(wall.x_pos - wall.thickness / 2),
            static_cast<int>(wall.holl_height),
            static_cast<int>(wall.thickness),
            static_cast<int>(wall.holl_width)
        };
        SDL_RenderFillRect(sdl_renderer_, &horiz_holl);
    }

    // Ego
    SDL_SetRenderDrawColor(sdl_renderer_, 255, 241, 0, 255);
    SDL_Rect ego{	
		static_cast<int>(ego_position.x - config_.ego_thickness / 2),
		static_cast<int>(ego_position.y - config_.ego_thickness / 2),
		config_.ego_thickness,
		config_.ego_thickness
	};
	SDL_RenderFillRect(sdl_renderer_, &ego);

    // Food
    SDL_SetRenderDrawColor(sdl_renderer_, 255, 120, 28, 255);
    SDL_Rect food{	
		static_cast<int>(food_position.x - config_.food_thickness / 2),
		static_cast<int>(food_position.y - config_.food_thickness / 2),
		config_.food_thickness,
		config_.food_thickness
	};
	SDL_RenderFillRect(sdl_renderer_, &food);

    SDL_RenderPresent(sdl_renderer_);
}

SDL_Window* Renderer::GetWindow()
{
    return sdl_window_;
}

void Renderer::UpdateWindowTitle(int food_score, int wall_score)
{
    std::string title{"Total Score: " + std::to_string(food_score + wall_score)
                     + " (Food Score: " + std::to_string(food_score)
                     + " Wall Score: " + std::to_string(wall_score)
                     + ")"
                     };
    SDL_SetWindowTitle(sdl_window_, title.c_str());
}
