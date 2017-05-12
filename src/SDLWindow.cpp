#include <SDLWindow.hpp>
#include <SDLException.hpp>
#include <SDL.hpp>

#include <SDL.h>

using namespace tetra;

SDLWindow::SDLWindow(const SDL&)
{
    handle = SDL_CreateWindow(
        "tetra-creative",
        100,
        100,
        600,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (handle == nullptr)
    {
        throw SDLException("Error creating SDLWindow 'tetra-creative'");
    }
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

void
SDLWindow::gl_SwapWindow() noexcept
{
    SDL_GL_SwapWindow(handle);
}
