#include <iostream>
#include <random>
#include "game.h"

const int ego_thickness = 12;
const int wall_thickness = 12;
const int holl_width = 72;

Game::Game():sdl_window_(nullptr), sdl_renderer_(nullptr), is_running_(true), last_time_(0)
{

}

bool Game::Initialize(const std::size_t width, const std::size_t height)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        return false;
    }

    sdl_window_ = SDL_CreateWindow("Fugitive", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, width, height, 0);

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

    ego_position_.x = ego_thickness / 2.0f;
    ego_position_.y = height / 2.0f;

    wall_horiz_dir_ = -1;
    wall_horiz_position_.x = width + width / 2.0f;
    wall_horiz_position_.y = 0;
    
    holl_horiz_position_ = height / 2.0f - holl_width / 2.0f;

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
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_time_ + 8));

    float delta_time = (SDL_GetTicks() - last_time_) / 1000.0f;
    if (delta_time > 0.05f)
    {
        delta_time = 0.05f;
    }

    int w, h;
    SDL_GetWindowSize(sdl_window_, &w, &h);

    if (ego_dir_.x != 0)
    {
        if ((ego_position_.x <= 0 + 6 && ego_dir_.x < 0)
          || (ego_position_.x > w - 6 && ego_dir_.x > 0))
        {
            // pass
        } else {
            ego_position_.x += ego_dir_.x * 800.0f * delta_time;
        }
    }

    if (ego_dir_.y != 0)
    {
        if ((ego_position_.y <= 0 + 6 && ego_dir_.y < 0)
          || (ego_position_.y >= h - 6 && ego_dir_.y > 0))
        {
            // pass
        } else {
            ego_position_.y += ego_dir_.y * 800.0f * delta_time;
        }
    }

    wall_horiz_position_.x += wall_horiz_dir_ * 500.0f * delta_time;
    if (wall_horiz_dir_ < 0 && wall_horiz_position_.x < -w / 2.0f)
    {
        wall_horiz_position_.x = w + w / 2;

        std::random_device rnd{};
        holl_horiz_position_ = rnd() % h - holl_width / 2;
    } else if (wall_horiz_dir_ > 0 && wall_horiz_position_.x > w + w / 2.0f)
    {
        wall_horiz_dir_ = -1;
    }

    last_time_ = SDL_GetTicks();
}

void Game::Render()
{
    int w, h;
    SDL_GetWindowSize(sdl_window_, &w, &h);

    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(sdl_renderer_);

    SDL_SetRenderDrawColor(sdl_renderer_, 255, 255, 255, 255);
    SDL_Rect ball{	
		static_cast<int>(ego_position_.x - ego_thickness / 2),
		static_cast<int>(ego_position_.y - ego_thickness / 2),
		ego_thickness,
		ego_thickness
	};
	SDL_RenderFillRect(sdl_renderer_, &ball);

    SDL_Rect horiz_wall{
        static_cast<int>(wall_horiz_position_.x - wall_thickness / 2),
        static_cast<int>(wall_horiz_position_.y),
        12,
        h
    };
    SDL_RenderFillRect(sdl_renderer_, &horiz_wall);

    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x00, 0x00, 0xFF);
    SDL_Rect horiz_holl{
        static_cast<int>(wall_horiz_position_.x - wall_thickness / 2),
        static_cast<int>(holl_horiz_position_),
        wall_thickness,
        holl_width
    };
    SDL_RenderFillRect(sdl_renderer_, &horiz_holl);

    SDL_RenderPresent(sdl_renderer_);
}
