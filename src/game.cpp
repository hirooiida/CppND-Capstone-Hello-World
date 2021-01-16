#include <iostream>
#include <random>
#include "game.h"

const int ego_thickness = 15;
const int food_thickness = 30;
const int holl_width = 100;

Game::Game(const std::size_t width, const std::size_t height)
{
    is_running_ = true;
    last_time_ = 0;

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

    ego_position_.x = ego_thickness / 2.0f;
    ego_position_.y = height / 2.0f;

    food_position_.x = width / 2.0f;
    food_position_.y = height / 2.0f;

    Wall wall_1{width + width * 0.0f,
                -1,
                height / 2.0f - holl_width / 2.0f,
                150,
                550,
                12};
    
    Wall wall_2{width + width * 0.5f,
                -1,
                height / 2.0f + holl_width / 2.0f,
                150,
                550,
                12};

    Wall wall_3{width + width * 1.0f,
                -1,
                height / 2.0f + holl_width / 2.0f,
                150,
                550,
                12};

    walls_.push_back(wall_1);
    walls_.push_back(wall_2);
    walls_.push_back(wall_3);
}

void Game::Run(Controller controller)
{
    while(is_running_)
    {
        controller.HandleInput(is_running_, ego_dir_);
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
            ego_position_.x += ego_dir_.x * 700.0f * delta_time;
        }
    }

    if (ego_dir_.y != 0)
    {
        if ((ego_position_.y <= 0 + 6 && ego_dir_.y < 0)
          || (ego_position_.y >= h - 6 && ego_dir_.y > 0))
        {
            // pass
        } else {
            ego_position_.y += ego_dir_.y * 700.0f * delta_time;
        }
    }

    Vector2 f_diff;
    f_diff.x = ego_position_.x - food_position_.x;
    f_diff.y = ego_position_.y - food_position_.y;
    if (f_diff.x < 0) {f_diff.x *= -1;}
    if (f_diff.y < 0) {f_diff.y *= -1;}
    if (f_diff.x < (ego_thickness + food_thickness) / 2.0f
        && f_diff.y < (ego_thickness + food_thickness) / 2.0f)
    {
        std::random_device rnd{};
        food_position_.x = rnd() % static_cast<int>(w * 0.6f) + w * 0.2f;
        food_position_.y = rnd() % static_cast<int>(h * 0.6f) + h * 0.2f;
    }

    for (Wall &wall: walls_)
    {
        wall.x_pos += wall.x_dir * wall.speed * delta_time;
        if (wall.x_dir < 0 && wall.x_pos < -w / 2.0f)
        {
            wall.x_pos = w + w / 10;

            std::random_device rnd{};
            wall.holl_height = rnd() % h - wall.holl_width / 2.0f;
            wall.speed = 450.0f + rnd() % 100;

        } else if (wall.x_dir > 0 && wall.x_pos > w + w / 2.0f)
        {
            wall.x_dir = -1;
        }

        Vector2 diff;
        diff.x = ego_position_.x - wall.x_pos;
        diff.y = ego_position_.y - wall.holl_height;
        if (diff.x < 0) { diff.x *= -1; }
        if (diff.x < wall.thickness && (diff.y > wall.holl_width - ego_thickness || diff.y < 0))
        {
            std::cout << "Crash" << std::endl;
            is_running_ = false;
        }
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

    for (Wall &wall: walls_)
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
		static_cast<int>(food_position_.x - food_thickness / 2),
		static_cast<int>(food_position_.y - food_thickness / 2),
		food_thickness,
		food_thickness
	};
	SDL_RenderFillRect(sdl_renderer_, &food);

    SDL_RenderPresent(sdl_renderer_);
}
