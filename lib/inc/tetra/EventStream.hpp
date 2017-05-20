#ifndef EVENTSTREAM_HPP
#define EVENTSTREAM_HPP

#include <vector>
#include <functional>
#include <boost/any.hpp>

namespace tetra
{
    /**
     * This class represents a continuous stream of events.
     * Other classes are allowed to subscribe to events and publish new events.
     */
    class EventStream
    {
    public:
        // Fwd declare, defined below
        class AutoRemoveListener;
        using ObserverId = int;
        using Listener = std::function<void(const boost::any&)>;

        /**
         * Create a new event stream.
         */
        EventStream(int maxEventsPerUpdate = 100);

        /**
         * Push a new event into the stream.
         * This will be handled by the next call to update().
         */
        void push(boost::any event);

        /**
         * Dispatch all of the events currently in the stream to listeners.
         */
        void dispatch();

        /**
         * Add an event listener which will be notified each time an event of the
         * correct type is processed.
         * A listener is any type with a method signature like:
         *   void Listener::someEventHandler(const EventType& event)
         *
         * The listener is added like:
         *   auto autoRemove = stream.addListener(listener,
         *                                        &Listener::someEventHandler)
         * Note: if the "instance" is destroyed before the autoRemoveListener
         * is destroyed then it is possible to segfault. Therefore it's good
         * to have the listening class own the AutoRemoveListener instance.
         */
        template <class TListener, class EventType>
        AutoRemoveListener addListener(TListener& instance,
                                       void (TListener::*method)(const EventType&))
        {
            TListener* instancePtr = &instance;
            listeners.emplace_back(nextId(), [=](const boost::any& event)
            {
                if (event.type() == boost::typeindex::type_id<EventType>())
                {
                    (instancePtr->*method)(boost::any_cast<const EventType&>(event));
                }
            });
            return AutoRemoveListener(*this, listeners.back().first);;
        }

        /**
         * This class is responsible for automatically removing a listener
         * when it is destroyed.
         */
        class AutoRemoveListener
        {
        public:
            AutoRemoveListener(EventStream& stream, ObserverId id);
            AutoRemoveListener(const AutoRemoveListener& stream) = delete;
            AutoRemoveListener(AutoRemoveListener&& autoRemovable);
            ~AutoRemoveListener();
        private:
            bool shouldRemove;
            ObserverId toRemove;
            EventStream& stream;
        };

    private:
        std::vector<boost::any> events;
        std::vector<std::pair<ObserverId, Listener>> listeners;
        ObserverId lastId = 0;
        const int maxEventsPerUpdate;

        ObserverId nextId();
        void remove(ObserverId id);
    };
} /* namespace tetra */

#endif
