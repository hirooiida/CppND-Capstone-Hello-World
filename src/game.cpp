#include <iostream>
#include <random>
#include "game.h"

const float ego_speed = 720.0f;
const float base_wall_speed = 400.0f;
const int dist_wall_speed = 20;
static float accel_wall = 0.0f;
float inc_wall_speed = 10.0f;

Game::Game(Config config)
{
    is_running_ = true;
    last_time_ = 0;
    food_score_ = 0;
    wall_score_ = 0;

    config_ = config;

    ego_position_.x = config_.ego_thickness / 2.0f;
    ego_position_.y = config_.screen_height / 2.0f;

    food_position_.x = config_.screen_width / 2.0f;
    food_position_.y = config_.screen_height / 2.0f;

    Wall wall_1{config_.screen_width + config_.screen_width * 0.0f,        // x_pos
                -1,                                                        // x_dir
                config_.screen_height / 2.0f - config_.holl_width / 2.0f,  // holl_height
                config_.holl_width,                                        // holl_width
                base_wall_speed,                                           // speed
                config_.wall_thickness                                     // thickness
                };
    
    Wall wall_2{config_.screen_width + config_.screen_width * 0.5f,
                -1,
                config_.screen_height / 2.0f + config_.holl_width / 2.0f,
                config_.holl_width,
                base_wall_speed,
                config_.wall_thickness};

    Wall wall_3{config_.screen_width + config_.screen_width * 1.0f,
                -1,
                config_.screen_height / 2.0f + config_.holl_width / 2.0f,
                config_.holl_width,
                base_wall_speed,
                config_.wall_thickness};

    walls_.push_back(wall_1);
    walls_.push_back(wall_2);
    walls_.push_back(wall_3);
}

void Game::Run(Controller controller, Renderer renderer)
{
    while(is_running_)
    {
        controller.HandleInput(is_running_, ego_dir_);
        UpdateGame();
        renderer.Render(ego_position_, walls_, food_position_);
        renderer.UpdateWindowTitle(food_score_, wall_score_);
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

    if (ego_dir_.x != 0)
    {
        if ((ego_position_.x <= 0 + config_.ego_thickness / 2 && ego_dir_.x < 0)
          || (ego_position_.x > config_.screen_width - config_.ego_thickness / 2 && ego_dir_.x > 0))
        {
            // pass
        } else {
            ego_position_.x += ego_dir_.x * ego_speed * delta_time;
        }
    }

    if (ego_dir_.y != 0)
    {
        if ((ego_position_.y <= 0 + config_.ego_thickness / 2 && ego_dir_.y < 0)
          || (ego_position_.y >= config_.screen_height - config_.ego_thickness / 2 && ego_dir_.y > 0))
        {
            // pass
        } else {
            ego_position_.y += ego_dir_.y * ego_speed * delta_time;
        }
    }

    Vector2 f_diff;
    f_diff.x = ego_position_.x - food_position_.x;
    f_diff.y = ego_position_.y - food_position_.y;
    if (f_diff.x < 0) {f_diff.x *= -1;}
    if (f_diff.y < 0) {f_diff.y *= -1;}
    if (f_diff.x < (config_.ego_thickness + config_.food_thickness) / 2.0f
        && f_diff.y < (config_.ego_thickness + config_.food_thickness) / 2.0f)
    {
        food_score_ += 10;
        std::random_device rnd{};
        food_position_.x = rnd() % static_cast<int>(config_.screen_width * 0.5f) + config_.screen_width * 0.15f;
        food_position_.y = rnd() % static_cast<int>(config_.screen_height * 0.5f) + config_.screen_height * 0.15f;
    }

    for (Wall &wall: walls_)
    {
        wall.x_pos += wall.x_dir * wall.speed * delta_time;
        if (wall.x_dir < 0 && wall.x_pos < -(config_.screen_width / 2.0f))
        {   
            ++wall_score_;
            accel_wall += inc_wall_speed;
            if (accel_wall > base_wall_speed) {inc_wall_speed = 0;}

            wall.x_pos = config_.screen_width + config_.screen_width / 10;

            std::random_device rnd{};
            wall.holl_height = rnd() % config_.screen_height - wall.holl_width / 2.0f;
            wall.speed = (base_wall_speed - dist_wall_speed / 2.0f) + rnd() % dist_wall_speed + accel_wall;

        } else if (wall.x_dir > 0 && wall.x_pos > config_.screen_width + config_.screen_width / 2.0f)
        {
            wall.x_dir = -1;
        }

        Vector2 diff;
        diff.x = ego_position_.x - wall.x_pos;
        diff.y = ego_position_.y - wall.holl_height;
        if (diff.x < 0) { diff.x *= -1; }
        if (diff.x < wall.thickness && (diff.y > wall.holl_width - config_.ego_thickness || diff.y < 0))
        {
            is_running_ = false;
        }
    }

    last_time_ = SDL_GetTicks();
}

int Game::GetTotalScore()
{
    return food_score_ + wall_score_;
}

int Game::GetFoodScore()
{
    return food_score_;
}

int Game::GetWallScore()
{
    return wall_score_;
}
