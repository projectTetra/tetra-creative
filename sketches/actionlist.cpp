#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <algorithm>

using namespace std;

class Action;
class ActionList;

class Action
{
public:
    bool complete = false;
    bool blocking = false;
    ActionList* parentList;

    virtual void run(float dt) = 0;
    virtual void onStart() = 0;
    virtual void onEnd() = 0;
};

class DebugAction : public Action
{
public:
    DebugAction(string msg, int runs);
    virtual void run(float dt) override;
    virtual void onStart() override;
    virtual void onEnd() override;

private:
    string msg;
    int runs;
};

unique_ptr<DebugAction> debug(const string& str, int runs = 1)
{
    return unique_ptr<DebugAction>{new DebugAction{str, runs}};
}

class ActionList
{
public:
    using OwnedAction = std::unique_ptr<Action>;

    Action* insertBefore(const Action* here, OwnedAction&& action);
    Action* insertAfter(const Action* here, OwnedAction&& action);
    Action* pushFront(OwnedAction&& action);

    void run(float dt);
private:
    using ActionCollection = std::vector<OwnedAction>;
    ActionCollection actions;

    /**
     * Insert the action at a location, set the parentList, and call onStart.
     */
    Action* insertAt(ActionCollection::iterator location, OwnedAction&& action);
};

int main()
{
    auto mylist = ActionList{};

    auto inserted = mylist.insertBefore(nullptr, debug("aoeu"));
    auto first = mylist.insertBefore(inserted, debug("first"));
    mylist.insertAfter(first, debug("second", 3));
    mylist.insertAfter(nullptr, debug("run twice", 2));

    cout << "start cycle{ " << endl;
    mylist.run(0.0f);
    cout << "}end" << endl;

    cout << "start cycle{ " << endl;
    mylist.run(0.2f);
    cout << "}end" << endl;

    cout << "start cycle{ " << endl;
    mylist.run(0.4f);
    cout << "}end" << endl;

    cout << "start cycle{ " << endl;
    mylist.run(0.6f);
    cout << "}end" << endl;

    return 0;
}

DebugAction::DebugAction(string msg, int runs) : msg{msg}, runs{runs} {}

void
DebugAction::run(float dt)
{
    cout << "debug action run: " << this->msg << " -- " << dt << endl;
    this->runs -= 1;

    if (runs <= 0)
    {
        this->complete = true;
    }
}

void
DebugAction::onStart()
{
    cout << "debug action start " << msg << endl;
}

void
DebugAction::onEnd()
{
    cout << "debug action end " << msg << endl;
}

Action*
ActionList::insertBefore(const Action* here, OwnedAction&& action)
{
    auto insertIter = find_if(begin(this->actions), end(this->actions),
                              [&here](const OwnedAction& action)
                              {
                                  return action.get() == here;
                              });
    return this->insertAt(insertIter, move(action));
}

Action*
ActionList::insertAfter(const Action* here, OwnedAction&& action)
{
    auto insertIter = find_if(begin(this->actions), end(this->actions),
                              [&here](const OwnedAction& action)
                              {
                                  return action.get() == here;
                              });
    if (insertIter != end(this->actions))
    {
        insertIter++;
    }
    return this->insertAt(insertIter, move(action));
}

Action*
ActionList::pushFront(OwnedAction&& action)
{
    return insertAt(begin(this->actions), move(action));
}

Action*
ActionList::insertAt(ActionCollection::iterator location, OwnedAction&& action)
{
    auto insertedIter = this->actions.insert(location, move(action));
    auto actionPtr = insertedIter->get();
    actionPtr->parentList = this;
    actionPtr->onStart();
    return actionPtr;
}

void
ActionList::run(float dt)
{
    for (const auto& action : actions)
    {
        action->run(dt);
        if (action->blocking)
        {
            break;
        }
    }

    // remove completed
    auto startOfCompleted = partition(begin(this->actions), end(this->actions),
                                      [](const OwnedAction& action)
                                      {
                                          return !action->complete;
                                      });
    // run onEnd for completed actions
    for_each(startOfCompleted, end(this->actions), [](const OwnedAction& action)
    {
        action->onEnd();
    });
    // erase the ended actions
    this->actions.erase(startOfCompleted, end(this->actions));
}

