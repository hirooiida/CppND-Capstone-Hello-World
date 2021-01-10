#include <iostream>
#include "game.h"

Game::Game():sdl_window_(nullptr), sdl_renderer_(nullptr), is_running_(true)
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
                                   SDL_WINDOWPOS_CENTERED, 768, 1024, 0);

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
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
        {
            is_running_ = false;
            break;
        }
    }
}

void Game::UpdateGame()
{

}

void Game::Render()
{
    SDL_SetRenderDrawColor(sdl_renderer_, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(sdl_renderer_);
    SDL_RenderPresent(sdl_renderer_);
}