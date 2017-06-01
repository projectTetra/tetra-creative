#include <tetra/behavior/Debug.hpp>
#include <tetra/behavior/Timing.hpp>
#include <tetra/TicTocClock.hpp>

#include <algorithm>
#include <iostream>

using namespace std;
using namespace tetra;

class MyTurboBehavior : public Behavior
{
public:
    virtual void onStart() override
    {
        cout << " -> starting my turbo behavior" << endl;
    }

    virtual void onEnd() override
    {
        cout << " -> ending my turbo behavior" << endl;
    }

    virtual void run(double dt) override
    {
        cout << " -> count is " << this->count << " wait 1 second" << endl;

        this->insertInFrontOfMe(Delay::forSeconds(1.0));
        this->insertInFrontOfMe(
            Echo::message("finished wait for count " + to_string(this->count))
        );
        this->count -= 1;

        if (this->count == 0)
        {
            this->complete = true;
            return;
        }
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
    auto totalTimer = HighResTicToc{};

    auto frameTimer = HighResTicToc{};

    auto mylist = BehaviorList{};
    mylist.pushFront(MyTurboBehavior::turbo());

    while(mylist.running())
    {
        mylist.run(frameTimer.ticToc());
    }

    cout << "completed in " << totalTimer.toc() << " seconds" << endl;
    /*
     * Did you notice that the total time is _more_ than 5 seconds?!
     * This is actually not incorrect behavior, there are two factors at play here:
     *
     * 1 - The delay is inserted by my turbo behavior _before_ itself.
     *     This means that the delay only starts burning down on the next frame.
     *     This means that a delay of 1 second which is created this frame, will
     *     actually end 1 second + ~16 milliseconds from now.
     *     The behavior is expected -- once the delay starts ticking it expires
     *     after 1 second.
     *
     * 2 - The c++ standard this_thread::sleep_for is used inside of the TicTocClock
     *     to sleep if the min duration hasn't been met.
     *     The sleep_for method is allowed to be somewhat inprecise and sleep for
     *     longer than requested. So depending on the std lib implementation and
     *     the underlying OS there is a bit of room for variablility.
     */
    return 0;
}

