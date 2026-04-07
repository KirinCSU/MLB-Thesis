#include <iostream>
#include <chrono>

#include "BookEvent.h"
#include "EventQueue.h"
#include "Publisher.h"
#include "Spec.h"
#include "Subscriber.h"
#include "Time.h"

using namespace MLB;

class NullSubscriber : public Subscriber
{
public:
    Int64 count;

    NullSubscriber()
        : count(0)
    {
    }

    void onEvent(const BookEvent* event)
    {
        count++;

        volatile OrderReferenceNumber orn = event->orn;
        volatile int size = event->size();
        volatile double price = event->price();
        (void)orn;
        (void)size;
        (void)price;
    }
};

class NullPublisher : public Publisher
{
public:
    Int64 count;

    NullPublisher()
        : count(0)
    {
    }

    void nextEvent()
    {
        count++;
    }
};

int main(int argc, char* argv[])
{
    Int64 numEvents = 1000000;

    if (argc >= 2)
    {
        numEvents = atoll(argv[1]);
    }

    std::cout << "Creating stress test with " << numEvents << " events" << std::endl;

    NullPublisher pub;
    NullSubscriber sub;
    EventQueue queue;
    queue.addSubscriber(&sub);

    Spec spec("AAPL", 0);

    typedef std::chrono::high_resolution_clock Clock;

    Clock::time_point t0 = Clock::now();

    // Insert in reverse chronological order so current queue insertion
    // does not become artificially quadratic for this test.
    for (Int64 i = numEvents - 1; i >= 0; --i)
    {
        Time t;
        t.sec = i / 1000000;
        t.usec = i % 1000000;

        BookEvent* event = new BookEvent(&pub, t);
        event->setAddition(&spec,
                           (OrderReferenceNumber)(1000000000ULL + i),
                           Buy,
                           100,
                           150.25);

        queue.insertEvent(event);
    }

    Clock::time_point t1 = Clock::now();

    while (queue.nextEvent())
    {
    }

    Clock::time_point t2 = Clock::now();

    double insertSecs = std::chrono::duration<double>(t1 - t0).count();
    double drainSecs  = std::chrono::duration<double>(t2 - t1).count();
    double totalSecs  = std::chrono::duration<double>(t2 - t0).count();

    std::cout << "Inserted:   " << numEvents << " events in " << insertSecs << " sec" << std::endl;
    std::cout << "Dispatched: " << sub.count << " events in " << drainSecs << " sec" << std::endl;
    std::cout << "Publisher callbacks: " << pub.count << std::endl;
    std::cout << "Total time: " << totalSecs << " sec" << std::endl;

    if (totalSecs > 0.0)
    {
        std::cout << "Throughput: " << (double)numEvents / totalSecs << " events/sec" << std::endl;
    }

    return 0;
}
