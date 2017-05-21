#include <tetra/NDCMouse.hpp>

using namespace std;
using namespace tetra;

NDCMouse::NDCMouse(EventStream& eventStream)
    : mouseMove{eventStream.addListener(*this, &NDCMouse::onMouseMove)}
    , screenResize{eventStream.addListener(*this, &NDCMouse::onScreenResize)}
    , width{1.0f}
    , height{1.0f}
    , _position{0.0, 0.0}
{ }

void
NDCMouse::onScreenResize(const SDLWindowSize& sizeEvent)
{
    width = sizeEvent.width;
    height = sizeEvent.height;
}

void
NDCMouse::onMouseMove(const SDLMousePosition& mousePosition)
{
    _position = {
        (mousePosition.x/width - 0.5f)*2.0f,
        (0.5f - mousePosition.y/height)*2.0f
    };
}

const array<float, 2>&
NDCMouse::position()
{
    return _position;
}
