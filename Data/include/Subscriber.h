#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <vector>

namespace MLB
{

class BookEvent;

class Subscriber
{
public:
    Subscriber() {}
    virtual ~Subscriber() {}

    virtual void onEvent(const BookEvent* event) {}
};

typedef std::vector<Subscriber*> Subscribers;

void FireEvent(const Subscribers& subs, const BookEvent* event);

}

#endif
