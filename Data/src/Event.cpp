#include "Event.h"

MLB::Event::Event(Publisher* a_publisher, Time a_eventTime, Type a_eventType)
    : publisher(a_publisher),
      eventTime(a_eventTime),
      eventType(a_eventType),
      next(0)
{
}

MLB::Event::~Event()
{
}

MLB::Event* MLB::Event::link(Event* nextEvent)
{
    next = nextEvent;
    return next;
}
