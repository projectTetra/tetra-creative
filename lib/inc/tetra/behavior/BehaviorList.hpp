#pragma once
#ifndef BEHAVIOR_LIST_HPP
#define BEHAVIOR_LIST_HPP

#include <memory>
#include <list>

namespace tetra
{

class Behavior;
using OwnedBehavior = std::unique_ptr<Behavior>;

/**
 * The BehaviorList is a collection of behaviors which are executed each time
 * run() is invoked.
 */
class BehaviorList
{
public:
    /**
     * Insert the owned behavior before the here pointer.
     * If the 'here' pointer cannot be found then the behavior is inserted at the
     * end of the list.
     */
    Behavior* insertBefore(const Behavior* here, OwnedBehavior&& action);

    /**
     * Insert the owned behavior after the 'here' pointer.
     * If the 'here' pointer cannot be found in the behavior list then the provided
     * behavior is inserted at the end of the list.
     */
    Behavior* insertAfter(const Behavior* here, OwnedBehavior&& action);

    /**
     * Push the provided behavior onto the front of the behavior list.
     */
    Behavior* pushFront(OwnedBehavior&& action);

    /**
     * Execute the behaviors in the behavior list, stopping at the end of the list
     * or after the first blocking Behavior executes.
     */
    void run(double dt);

    /**
     * Returns true as long as there are any behaviors which have not completed.
     */
    bool running() const;
private:
    using BehaviorCollection = std::list<OwnedBehavior>;
    BehaviorCollection behaviors;

    /**
     * Insert the action at a location, set the parentList, and call onStart.
     */
    Behavior* insertAt(BehaviorCollection::iterator location, OwnedBehavior&& action);
};

}; /* namespace tetra */

#endif
