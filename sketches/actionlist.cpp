#include <tetra/TicTocClock.hpp>

#include <algorithm>
#include <iostream>
#include <list>

using namespace std;
using namespace tetra;

class Behavior;
class BehaviorList;
using OwnedBehavior = std::unique_ptr<Behavior>;

/**
 * A behavior is a chunk of code which executes in a BehaviorList.
 * Behaviors are provided a pointer to their parent list before their
 * onStart method is first called.
 */
class Behavior
{
public:
    bool complete = false;
    bool blocking = false;
    BehaviorList* parentList;

    virtual void run(double dt) = 0;
    virtual void onStart();
    virtual void onEnd();

    Behavior* insertInFrontOfMe(OwnedBehavior&& ownedBehavior);
};

void Behavior::onStart() {}
void Behavior::onEnd() {}

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

Behavior*
Behavior::insertInFrontOfMe(OwnedBehavior&& ownedBehavior)
{
    return this->parentList->insertBefore(this, move(ownedBehavior));
}

class Delay : public Behavior
{
public:
    Delay(double duration);

    virtual void run(double dt) override;

    static unique_ptr<Delay> forSeconds(double duration);
private:
    const double duration;
    double elapsed;
};

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

class Echo : public Behavior
{
public:
    Echo(const std::string& msg);

    virtual void run(double dt) override;

    template <class T>
    static unique_ptr<Echo> message(const T& msg)
    {
        using std::to_string;
        return unique_ptr<Echo>{new Echo{to_string(msg)}};
    }

    static unique_ptr<Echo> message(const char* msg);
private:
    const std::string msg;
};

Echo::Echo(const std::string& msg)
    : msg{msg}
{ }

void
Echo::run(double dt)
{
    cout << msg << endl;
    this->complete = true;
}

unique_ptr<Echo>
Echo::message(const char* msg)
{
    return unique_ptr<Echo>{new Echo{string{msg}}};
}

class MyTurboBehavior : public Behavior
{
public:

    virtual void run(double dt) override
    {
        if (count == 0)
        {
            this->complete = true;
            return;
        }

        this->insertInFrontOfMe(Echo::message("HEHEHEHEHE"));
        this->insertInFrontOfMe(Delay::forSeconds(1.0));
        this->insertInFrontOfMe(Echo::message(count));
        count -= 1;
    }

    static unique_ptr<MyTurboBehavior> turbo()
    {
        return unique_ptr<MyTurboBehavior>{new MyTurboBehavior{}};
    }
private:
    int count = 5;
};

int main()
{
    HighResTicToc timer{};

    auto mylist = BehaviorList{};
    mylist.pushFront(MyTurboBehavior::turbo());

    while(mylist.running())
    {
        mylist.run(timer.ticToc());
    }
    return 0;
}

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
    auto actionPtr = insertedIter->get();
    actionPtr->parentList = this;
    actionPtr->onStart();
    return actionPtr;
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


