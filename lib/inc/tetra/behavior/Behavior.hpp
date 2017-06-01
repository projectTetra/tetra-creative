#pragma once
#ifndef BEHAVIOR_HPP
#define BEHAVIOR_HPP

#include <tetra/behavior/BehaviorList.hpp>

namespace tetra
{

/**
 * A behavior is a chunk of code which executes in a BehaviorList.
 * Behaviors are provided a pointer to their parent list before their
 * onStart method is first called.
 */
class Behavior
{
public:
    bool blocking = false;
    bool complete = false;
    BehaviorList* parentList;

    virtual void run(double dt) = 0;
    virtual void onStart();
    virtual void onEnd();

    Behavior* insertInFrontOfMe(OwnedBehavior&& ownedBehavior);
};

}; /* namespace tetra */

#endif
