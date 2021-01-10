#include <iostream>
#include "game.h"

Game::Game():sdl_window_(nullptr), sdl_renderer_(nullptr), is_running_(true), last_time_(0)
{

}

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    sdl_window_ = SDL_CreateWindow("Fugitive", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, 800, 800, 0);

    if (nullptr == sdl_window_) {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == sdl_renderer_) {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    ego_position_.x = 400.0f;
    ego_position_.y = 400.0f;

    return true;
}

void Game::Run()
{
    while(is_running_)
    {
        HandleInput();
        UpdateGame();
        Render();
    } 
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(sdl_renderer_);
	SDL_DestroyWindow(sdl_window_);
	SDL_Quit();
}

void Game::HandleInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            is_running_ = false;
            break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_ESCAPE])
	{
		is_running_ = false;
	}

	ego_dir_ = {0, 0};
	if (state[SDL_SCANCODE_W])
	{
		ego_dir_.y -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		ego_dir_.y += 1;
	}
    if (state[SDL_SCANCODE_D])
	{
		ego_dir_.x += 1;
	}
    if (state[SDL_SCANCODE_A])
	{
		ego_dir_.x -= 1;
	}
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_time_ + 16));

    float delta_time = (SDL_GetTicks() - last_time_) / 1000.0f;
    if (delta_time > 0.05f)
    {
        delta_time = 0.05f;
    }

    if (ego_dir_.x != 0)
    {
        ego_position_.x += ego_dir_.x * 1000.0f * delta_time;
    }
    if (ego_dir_.y != 0)
    {
        ego_position_.y += ego_dir_.y * 1000.0f * delta_time;
    }

    last_time_ = SDL_GetTicks();

}

void Game::Render()
{
    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(sdl_renderer_);

    SDL_SetRenderDrawColor(sdl_renderer_, 255, 255, 255, 255);
    SDL_Rect ball{	
		static_cast<int>(ego_position_.x - 5),
		static_cast<int>(ego_position_.y - 5),
		10,
		10
	};
	SDL_RenderFillRect(sdl_renderer_, &ball);

    SDL_RenderPresent(sdl_renderer_);
}
