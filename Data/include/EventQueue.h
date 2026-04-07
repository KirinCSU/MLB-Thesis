#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Event.h"
#include "Subscriber.h"
#include <deque>

namespace MLB
{

class EventQueue
{
private:
    typedef std::deque<Event*> Events;

private:
    Subscribers subscribers;
    Events events;

public:
    EventQueue();
    EventQueue(const EventQueue& rhs);
    ~EventQueue();

    // Modifiers
    bool nextEvent();
    void insertEvent(Event* event);
    void addSubscriber(Subscriber* sub);

    // Manual Control
    bool haveEvent();
    Event* getNextEvent();
};

}

#endif
