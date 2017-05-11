#include <SDL.hpp>

#include <SDL.h>

using namespace tetra;

SDL::SDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    this->shouldQuit = true;
}

SDL::SDL(SDL&& from)
{
    from.shouldQuit = false;
}

SDL::~SDL()
{
    if (this->shouldQuit)
    {
        SDL_Quit();
    }
}
