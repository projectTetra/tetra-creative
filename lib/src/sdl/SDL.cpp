#include <sdl/SDL.hpp>
#include <sdl/SDLEvents.hpp>

#include <SDL.h>

using namespace tetra;

SDL::SDL(EventStream& eventStream)
    : eventStream{eventStream}
    , quitReceived{false}
{
    SDL_Init(SDL_INIT_EVERYTHING);
    this->shouldDestroySDL = true;
}

SDL::SDL(SDL&& from)
    : eventStream{from.eventStream}
    , quitReceived{from.quitReceived}
{
    // don't let someone else destroy SDL because we are still using it!
    from.shouldDestroySDL = false;
}

SDL::~SDL()
{
    if (shouldDestroySDL)
    {
        SDL_Quit();
    }
}

namespace
{
    void pushWindowEvent(EventStream& eventStream, SDL_Event& event)
    {
        switch (event.window.event)
        {
            case SDL_WINDOWEVENT_RESIZED:
                eventStream.push(
                    SDLWindowSize{event.window.data1, event.window.data2}
                );
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                eventStream.push(
                    SDLWindowSize{event.window.data1, event.window.data2}
                );
                break;
        }
    }
}

void
SDL::pushEvents()
{
    auto event = SDL_Event{};
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            eventStream.push(SDLQuit{});
            quitReceived = true;
        }
        else if (event.type == SDL_WINDOWEVENT)
        {
            pushWindowEvent(eventStream, event);
        }
    }
}

bool
SDL::running() const
{
    return !quitReceived;
}

