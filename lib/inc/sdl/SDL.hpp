#ifndef SDL_HPP
#define SDL_HPP

#include <tetra/EventStream.hpp>

namespace tetra
{
    class SDL
    {
    public:
        SDL(EventStream& eventStream);
        SDL(const SDL&) = delete;
        SDL(SDL&&);
        ~SDL();

        /**
         * Poll for SDL events and push the typed versions into the event stream.
         */
        void pushEvents();

        /**
         * True until the first SDL_QUIT message.
         */
        bool running() const;
    private:
        bool shouldDestroySDL;
        bool quitReceived;
        EventStream& eventStream;
    };
}; /* namespace tetra */

#endif
