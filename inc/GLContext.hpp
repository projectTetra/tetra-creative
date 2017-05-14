#ifndef GLCONTEXT_HPP
#define GLCONTEXT_HPP

#include <SDL.h>

namespace tetra
{
    class GLContext
    {
    public:
        class Builder
        {
        public:
            Builder(SDL_Window*);
            Builder(const Builder&) = default;
            Builder(Builder&&) = default;

            /**
             * Set the GLContext major version -- defaults to 3.
             */
            Builder& majorVersion(int version);

            /**
             * Set the GLContext minor version -- defaults to 1.
             */
            Builder& minorVersion(int version);

            /**
             * Set the SDL_GL_CONTEXT_PROFILE_MASK -- defaults to CORE.
             */
            Builder& profileMask(int mask);

            /**
             * Construct a GLContext for a window.
             */
            GLContext build();

        private:
            int _majorVersion;
            int _minorVersion;
            int _profileMask;
            SDL_Window* _window;
        };

        GLContext(GLContext&&);
        GLContext(const GLContext&) = delete;
        ~GLContext();
    private:
        GLContext(SDL_GLContext);

        SDL_GLContext context;
    };
} /* namespac tetra */

#endif
