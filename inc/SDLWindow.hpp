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

        /**
         * Call SDL_GL_SwapWindow with the underlying window.
         */
        void gl_SwapWindow() noexcept;
    private:
        SDL_Window* handle;
    };
}; /* namespace tetra */

#endif
