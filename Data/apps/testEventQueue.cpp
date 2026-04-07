#include <iostream>

#include "BookEvent.h"
#include "EventQueue.h"
#include "Publisher.h"
#include "Spec.h"
#include "Subscriber.h"
#include "Time.h"

using namespace MLB;

class TestSubscriber : public Subscriber
{
public:
    void onEvent(const BookEvent* event)
    {
        std::cout << "Dispatch: "
                  << "Time=" << event->eventTime.str(true)
                  << " ORN=" << event->orn
                  << " Size=" << event->size()
                  << " Price=" << event->price()
                  << std::endl;
    }
};

class TestPublisher : public Publisher
{
public:
    int calls;

    TestPublisher()
        : calls(0)
    {
    }

    void nextEvent()
    {
        calls++;
        std::cout << "Publisher nextEvent called, count=" << calls << std::endl;
    }
};

int main()
{
    TestPublisher pub;
    TestSubscriber sub;

    EventQueue queue;
    queue.addSubscriber(&sub);

    Spec spec("AAPL", 0);

    Time t1;
    t1.sec = 9 * 3600 + 30 * 60;
    t1.usec = 300;

    Time t2;
    t2.sec = 9 * 3600 + 30 * 60;
    t2.usec = 100;

    Time t3;
    t3.sec = 9 * 3600 + 30 * 60;
    t3.usec = 200;

    BookEvent* e1 = new BookEvent(&pub, t1);
    BookEvent* e2 = new BookEvent(&pub, t2);
    BookEvent* e3 = new BookEvent(&pub, t3);

    e1->setAddition(&spec, 1001, true, 10, 150.25);
    e2->setExecution(&spec, 1001, 5);
    e3->setDeletion(&spec, 1001);

    queue.insertEvent(e1);
    queue.insertEvent(e2);
    queue.insertEvent(e3);

    while (queue.nextEvent())
    {
    }

    std::cout << "Final publisher call count=" << pub.calls << std::endl;

    return 0;
}
