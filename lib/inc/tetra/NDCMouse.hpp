#ifndef NDCMOUSE_HPP
#define NDCMOUSE_HPP

#include <tetra/EventStream.hpp>
#include <sdl/SDLEvents.hpp>

#include <array>

namespace tetra
{
    /**
     * This class represents the mouse's current position in OpenGL NDC space.
     */
    class NDCMouse
    {
    public:
        NDCMouse(EventStream& eventStream);
        NDCMouse(const NDCMouse&) = delete;
        NDCMouse(NDCMouse&&) = default;

        void onScreenResize(const SDLWindowSize& sizeEvent);
        void onMouseMove(const SDLMousePosition& mousePosition);

        const std::array<float, 2>& position();
    private:
        float width, height;
        std::array<float, 2> _position;
        EventStream::AutoRemoveListener mouseMove;
        EventStream::AutoRemoveListener screenResize;
    };
};

#endif
