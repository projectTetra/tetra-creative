#include <tetra/behavior/Timing.hpp>

using namespace std;
using namespace tetra;

Delay::Delay(double duration)
    : duration{duration}
    , elapsed{0.0f}
{
    this->blocking = true;
}

void
Delay::run(double dt)
{
    elapsed += dt;
    if (elapsed >= duration)
    {
        this->complete = true;
    }
}

unique_ptr<Delay>
Delay::forSeconds(double duration)
{
    return unique_ptr<Delay>{new Delay{duration}};
}
