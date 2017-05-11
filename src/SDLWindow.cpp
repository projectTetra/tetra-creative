#include <SDLWindow.hpp>
#include <SDLException.hpp>
#include <SDL.hpp>

#include <SDL.h>

using namespace tetra;

SDLWindow::SDLWindow(const SDL&)
{
    handle = SDL_CreateWindow(
        "tetra-creative",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        600,
        600,
        SDL_WINDOW_SHOWN
    );

    if (handle == nullptr)
    {
        throw SDLException("Error creating SDLWindow 'tetra-creative'");
    }

    auto renderer = SDL_CreateRenderer(handle, 0, SDL_RENDERER_ACCELERATED);
    SDL_RenderPresent(renderer);
    SDL_DestroyRenderer(renderer);
}

SDLWindow::SDLWindow(SDLWindow&& from) noexcept
{
    std::swap(from.handle, handle);
    from.handle = nullptr;
}

SDLWindow::~SDLWindow()
{
    if (handle != nullptr)
    {
        SDL_DestroyWindow(handle);
    }
}
