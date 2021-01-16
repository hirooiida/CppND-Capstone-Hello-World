#include "SDL.h"
#include "controller.h"

void Controller::HandleInput(bool &is_running, Vector2 &dir)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            is_running = false;
            break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_ESCAPE])
	{
		is_running = false;
	}

	dir = {0, 0};
	if (state[SDL_SCANCODE_W])
	{
		dir.y -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		dir.y += 1;
	}
    if (state[SDL_SCANCODE_D])
	{
		dir.x += 1;
	}
    if (state[SDL_SCANCODE_A])
	{
		dir.x -= 1;
	}
}
