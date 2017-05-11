#ifndef SDL_WINDOW_HPP
#define SDL_WINDOW_HPP

#include <SDLfwd.hpp>
#include <SDL.h>

namespace tetra
{
    class SDLWindow
    {
    public:
        SDLWindow(const SDL& lib);
        SDLWindow(const SDLWindow&) = delete;
        SDLWindow(SDLWindow&&) noexcept;
        ~SDLWindow();

    private:
        SDL_Window* handle;
    };
}; /* namespace tetra */

#endif
