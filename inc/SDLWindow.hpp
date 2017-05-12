#ifndef SDL_WINDOW_HPP
#define SDL_WINDOW_HPP

#include <SDLfwd.hpp>
#include <SDL.h>

#include <string>

namespace tetra
{
    class SDLWindow
    {
    public:
        class Builder
        {
        public:
            Builder(const SDL&);

            /**
             * Set the window's initial x position -- defaults to SDL_WINDOWPOS_UNDEFINED.
             */
            Builder& x(int x);

            /**
             * Set the window's initial y position -- defaults to SDL_WINDOWPOS_UNDEFINED.
             */
            Builder& y(int y);

            /**
             * Set the window's initial width -- defaults to 800.
             */
            Builder& width(int w);

            /**
             * Set the window's initial height -- defaults to 600.
             */
            Builder& height(int h);

            /**
             * Set the window's title -- defaults to 'tetra-creative'.
             */
            Builder& title(const std::string& title);

            /**
             * Construct an SDLWindow.
             * @throws SDLException if there is an error while constructing the window.
             */
            SDLWindow build();

        private:
            int _x;
            int _y;
            int _w;
            int _h;
            std::string _title;
        };

        SDLWindow(const SDLWindow&) = delete;
        SDLWindow(SDLWindow&&) noexcept;
        ~SDLWindow();

        /**
         * Call SDL_GL_SwapWindow with the underlying window.
         */
        void gl_SwapWindow() noexcept;

    private:
        /**
         * Create a SDL OpenGL context for the window.
         * @throws SDLException if the context cannot be constructed for some reason.
         */
        void gl_CreateContext();

        SDLWindow(SDL_Window*);
        SDL_Window* handle;
    };
}; /* namespace tetra */

#endif
