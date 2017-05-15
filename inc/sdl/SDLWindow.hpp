#ifndef SDL_WINDOW_HPP
#define SDL_WINDOW_HPP

#include <sdl/SDL.hpp>
#include <sdl/GLContext.hpp>

#include <SDL.h>

#include <string>

namespace tetra
{
    /**
     * This class owns an instance of SDL_Window.
     */
    class SDLWindow
    {
    public:
        /**
         * This class is responsible for configuring and building a SDLWindow.
         */
        class Builder
        {
        public:
            /**
             * Create a new SDLWindow builder.
             */
            Builder();

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

        /**
         * This class represents the current frame which is being used as a render target.
         * Eventually there will probably be a framebuffer abstraction of some kind
         * which this will play with.
         */
        class Frame
        {
        public:
            /**
             * Create a new frame for the window.
             */
            Frame(SDLWindow&);

            /**
             * Transfer ownership of the frame.
             */
            Frame(Frame&&);

            /**
             * Frames cannot be copied.
             */
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

        /**
         * Windows cannot be copied.
         */
        SDLWindow(const SDLWindow&) = delete;

        /**
         * Transfer ownership of the window.
         */
        SDLWindow(SDLWindow&& from) noexcept;

        /**
         * Destroy the window.
         */
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

        /**
         * Get a non-owning handle to the underlyng SDL_Window pointer.
         */
        SDL_Window* raw() const;
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
         * Take ownership of a SDL_Window* handle.
         */
        SDLWindow(SDL_Window* handle);

        SDL_Window* handle;
    };
}; /* namespace tetra */

#endif
