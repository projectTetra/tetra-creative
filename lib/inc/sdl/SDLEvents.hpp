#ifndef SDLEVENTS_HPP
#define SDLEVENTS_HPP

namespace tetra
{
    /** This event represents an SDL_QUIT */
    struct SDLQuit {};

    /** This event is fired when a window's size changes */
    struct SDLWindowSize{int width; int height;};

    /** This event is fired when the mouse's position changes */
    struct SDLMousePosition
    {
        int x; /** Mouse x position */
        int y; /** Mouse y position */
        int relx; /** Difference between last x and current x */
        int rely; /** Difference between last y and current y */
    };
}; /* namespace tetra */

#endif
