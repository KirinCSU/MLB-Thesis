#include "Subscriber.h"
#include "BookEvent.h"

void MLB::FireEvent(const Subscribers& subs, const BookEvent* event)
{
    const BookEvent* iter = event;

    while (iter)
    {
        Subscribers::const_iterator s = subs.begin();
        while (s != subs.end())
        {
            (*s)->onEvent(iter);
            ++s;
        }

        iter = (const BookEvent*)iter->next;
    }
}
