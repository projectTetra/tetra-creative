#include <tetra/behavior/Behavior.hpp>

using namespace tetra;

void Behavior::onStart() {}
void Behavior::onEnd() {}

Behavior*
Behavior::insertInFrontOfMe(OwnedBehavior&& ownedBehavior)
{
    return this->parentList->insertBefore(this, move(ownedBehavior));
}

