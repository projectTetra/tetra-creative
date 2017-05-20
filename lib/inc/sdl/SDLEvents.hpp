#ifndef SDLEVENTS_HPP
#define SDLEVENTS_HPP

namespace tetra
{
    /** This event represents an SDL_QUIT */
    struct SDLQuit {};

    /** This event is fired when a window's size changes */
    struct SDLWindowSize{int width; int height;};
}; /* namespace tetra */

#endif
