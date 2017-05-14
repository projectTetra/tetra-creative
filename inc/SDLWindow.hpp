#ifndef SDL_WINDOW_HPP
#define SDL_WINDOW_HPP

#include <GLContext.hpp>
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

        class Frame
        {
        public:
            Frame(SDLWindow&);
            Frame(Frame&&);
            Frame(const Frame&) = delete;

            /**
             * Calls complete() to complete the frame.
             */
            ~Frame();

            /**
             * Swap the window's back buffers to present the screen.
             * Only the first call to complete() will have effect, any further calls
             * will have no effect.
             */
            void complete();
        private:

            SDLWindow& window;
            bool completed;
        };

        SDLWindow(const SDLWindow&) = delete;
        SDLWindow(SDLWindow&&) noexcept;
        ~SDLWindow();

        /**
         * Create an OpenGL context builder for this window.
         */
        GLContext::Builder contextBuilder();

        /**
         * Draw ensures the window's opengl context is active and returns a Frame
         * object which is responsible for flipping the window buffer when it goes out
         * of scope or is completed.
         */
        Frame draw();
    private:
        /**
         * Create a SDL OpenGL context for the window.
         * @throws SDLException if the context cannot be constructed for some reason.
         */
        void gl_CreateContext();

        /**
         * Call SDL_GL_SwapWindow with the underlying window.
         */
        void gl_SwapWindow() noexcept;

        /**
         * Create a new SDLWindow which owns the provide SDL_Window handle.
         */
        SDLWindow(SDL_Window*);

        SDL_Window* handle;
    };
}; /* namespace tetra */

#endif
