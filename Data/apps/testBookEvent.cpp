#include <iostream>

#include "BookEvent.h"
#include "Subscriber.h"
#include "Publisher.h"
#include "Spec.h"
#include "Time.h"

using namespace MLB;

class TestSubscriber : public Subscriber
{
public:
    void onEvent(const BookEvent* event)
    {
        std::cout << "Event: ";

        if (event->isAddition())
        {
            std::cout << "Addition ";
        }
        else if (event->isExecution())
        {
            std::cout << "Execution ";
        }
        else if (event->isDeletion())
        {
            std::cout << "Deletion ";
        }

        std::cout << "ORN=" << event->orn
                  << " Size=" << event->size()
                  << " Price=" << event->price()
                  << std::endl;
    }
};

class DummyPublisher : public Publisher
{
public:
    void nextEvent()
    {
    }
};

int main()
{
    DummyPublisher pub;

    Spec spec("AAPL", 0);

    Time t1;
    t1.sec = 9 * 3600 + 30 * 60;
    t1.usec = 100;

    Time t2;
    t2.sec = 9 * 3600 + 30 * 60;
    t2.usec = 200;

    Time t3;
    t3.sec = 9 * 3600 + 30 * 60;
    t3.usec = 300;

    BookEvent* e1 = new BookEvent(&pub, t1);
    BookEvent* e2 = new BookEvent(&pub, t2);
    BookEvent* e3 = new BookEvent(&pub, t3);

    e1->setAddition(&spec, 1001, true, 10, 150.25);
    e2->setExecution(&spec, 1001, 5);
    e3->setDeletion(&spec, 1001);

    e1->link(e2)->link(e3);

    TestSubscriber sub;
    Subscribers subs;
    subs.push_back(&sub);

    FireEvent(subs, e1);

    delete e1;
    delete e2;
    delete e3;

    return 0;
}
