#include <tetra/EventStream.hpp>

#include <algorithm>

using namespace tetra;
using namespace std;
using namespace boost;

using AutoRemoveListener = EventStream::AutoRemoveListener;
using ObserverId = EventStream::ObserverId;

AutoRemoveListener::AutoRemoveListener(EventStream& stream, ObserverId id)
    : stream{stream}
    , toRemove{id}
    , shouldRemove{true}
{ }

AutoRemoveListener::AutoRemoveListener(AutoRemoveListener&& from)
    : stream{from.stream}
    , toRemove{from.toRemove}
    , shouldRemove{from.shouldRemove}
{
    from.shouldRemove = false;
}

AutoRemoveListener::~AutoRemoveListener()
{
    if (shouldRemove)
    {
        stream.remove(toRemove);
    }
}

EventStream::EventStream(int maxEventsPerUpdate)
    : maxEventsPerUpdate{maxEventsPerUpdate}
{ }

void
EventStream::push(boost::any event)
{
    events.push_back(event);
}

void
EventStream::dispatch()
{
    for(int count = 0; count < maxEventsPerUpdate && !events.empty(); count++)
    {
        for (const auto& listener : listeners)
        {
            listener.second(events[0]);
        }
        events.erase(events.begin());
    }
}

ObserverId
EventStream::nextId()
{
    lastId += 1;
    return lastId;
}

void
EventStream::remove(ObserverId id)
{
    listeners.erase(remove_if(begin(listeners), end(listeners),
        [&](std::pair<ObserverId, Listener>& handler) {
            return handler.first == id;
        }),
        end(listeners)
    );
}

