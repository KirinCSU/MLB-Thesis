#include "EventQueue.h"
#include "BookEvent.h"

MLB::EventQueue::EventQueue()
{
}

MLB::EventQueue::EventQueue(const EventQueue& rhs)
{
}

MLB::EventQueue::~EventQueue()
{
}

bool MLB::EventQueue::haveEvent()
{
    return !events.empty();
}

MLB::Event* MLB::EventQueue::getNextEvent()
{
    if (events.empty())
    {
        return 0;
    }

    Event* result = events.front();
    events.pop_front();
    return result;
}

bool MLB::EventQueue::nextEvent()
{
    if (events.empty())
    {
        return false;
    }

    Event* event = events.front();
    Publisher* publisher = event->publisher;

    FireEvent(subscribers, (const BookEvent*)event);

    events.pop_front();
    delete event;

    if (publisher)
    {
        publisher->nextEvent();
    }

    return true;
}

void MLB::EventQueue::insertEvent(Event* event)
{
    Events::iterator e = events.begin();

    while (e != events.end())
    {
        if (event->eventTime.sec < (*e)->eventTime.sec)
        {
            break;
        }

        if (event->eventTime.sec == (*e)->eventTime.sec &&
            event->eventTime.usec < (*e)->eventTime.usec)
        {
            break;
        }

        ++e;
    }

    events.insert(e, event);
}

void MLB::EventQueue::addSubscriber(Subscriber* sub)
{
    subscribers.push_back(sub);
}
