#ifndef EVENT_H
#define EVENT_H

#include "Publisher.h"
#include "Time.h"

namespace MLB
{

class Event
{
public:
    enum Type
    {
        Book
    };

public:
    Publisher* publisher;
    Time eventTime;
    Type eventType;
    Event* next;

public:
    Event(Publisher* publisher, Time eventTime, Type eventType);
    virtual ~Event();

    Event* link(Event* nextEvent);
};

}

#endif
