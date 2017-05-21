#include <tetra/AdaptiveOrtho.hpp>

using namespace std;
using namespace tetra;
using namespace glm;

AdaptiveOrtho::AdaptiveOrtho(EventStream& eventStream, float scale)
    : resizeScreen{eventStream.addListener(*this, &AdaptiveOrtho::onWindowSize)}
    , projection{1.0f}
    , scale{scale}
{ }

void
AdaptiveOrtho::onWindowSize(const SDLWindowSize& event)
{
    float aInv = (float)event.height/event.width;
    float invScale = 1.0/scale;

    projection = glm::mat4(
        invScale*aInv, 0.0,      0.0, 0.0,
        0.0,           invScale, 0.0, 0.0,
        0.0,           0.0,      1.0, 0.0,
        0.0,           0.0,      0.0, 1.0
    );
}

const mat4&
AdaptiveOrtho::value()
{
    return projection;
}
