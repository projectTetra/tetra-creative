#include <tetra/behavior/BehaviorList.hpp>
#include <tetra/behavior/Behavior.hpp>

#include <algorithm>

using namespace std;
using namespace tetra;

Behavior*
BehaviorList::insertBefore(const Behavior* here, OwnedBehavior&& action)
{
    auto insertIter = find_if(begin(this->behaviors), end(this->behaviors),
                              [&here](const OwnedBehavior& action)
                              {
                                  return action.get() == here;
                              });
    return this->insertAt(insertIter, move(action));
}

Behavior*
BehaviorList::insertAfter(const Behavior* here, OwnedBehavior&& action)
{
    auto insertIter = find_if(begin(this->behaviors), end(this->behaviors),
                              [&here](const OwnedBehavior& action)
                              {
                                  return action.get() == here;
                              });
    if (insertIter != end(this->behaviors))
    {
        insertIter++;
    }
    return this->insertAt(insertIter, move(action));
}

Behavior*
BehaviorList::pushFront(OwnedBehavior&& action)
{
    return insertAt(begin(this->behaviors), move(action));
}

Behavior*
BehaviorList::insertAt(BehaviorCollection::iterator location, OwnedBehavior&& action)
{
    auto insertedIter = this->behaviors.insert(location, move(action));
    auto behavior = insertedIter->get();
    behavior->parentList = this;
    behavior->onStart();
    return behavior;
}

void
BehaviorList::run(double dt)
{
    // For loop is stable even if behaviors insert elements because
    // std::list iterators are not invalidated when elements are inserted.
    // all removal happens after the update loop
    for (const auto& behavior : this->behaviors)
    {
        // don't allow a completed behavior to run or block others
        if (behavior->complete)
        {
            continue;
        }

        behavior->run(dt);
        if (behavior->blocking)
        {
            break;
        }
    }

    // move completed actions to the end of the list (note that stable partition
    // preserves relative ordering of behaviors)
    auto startOfCompleted =
        stable_partition(begin(this->behaviors), end(this->behaviors),
                         [](const OwnedBehavior& action)
                         {
                             return !action->complete;
                         });
    // run onEnd for completed actions
    for_each(startOfCompleted, end(this->behaviors), [](const OwnedBehavior& action)
    {
        action->onEnd();
    });
    // erase the ended actions
    this->behaviors.erase(startOfCompleted, end(this->behaviors));
}

bool
BehaviorList::running() const
{
    return !this->behaviors.empty();
}

