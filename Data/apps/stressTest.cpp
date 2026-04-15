#include <iostream>
#include <chrono>

#include "CompressedReader.h"
#include "EventQueue.h"
#include "Subscriber.h"
#include "BookEvent.h"

using namespace MLB;

class PipelineSubscriber : public Subscriber
{
public:
    Int64 count;

public:
    PipelineSubscriber()
        : count(0)
    {
    }

    void onEvent(const BookEvent* event)
    {
        count++;

        if (count % 10000 == 0)
        {
            std::cout << "Event ";

            if (event->isAddition())
            {
                std::cout << "Addition ";
            }
            else if (event->isDeletion())
            {
                std::cout << "Deletion ";
            }
            else if (event->isRevision())
            {
                std::cout << "Revision ";
            }
            else if (event->isExecution())
            {
                std::cout << "Execution ";
            }
            else if (event->isHidTrade())
            {
                std::cout << "HidTrade ";
            }

            if (event->spec)
            {
                std::cout << "Ticker=" << event->spec->ticker << " ";
            }

            std::cout << "Time=" << event->eventTime.str(true) << " "
                      << "ORN=" << event->orn << " "
                      << "Buy=" << event->buy() << " "
                      << "Size=" << event->size() << " "
                      << "Price=" << event->price()
                      << std::endl;
        }
    }
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Needs compressed file" << std::endl;
        return 1;
    }

    EventQueue queue;
    PipelineSubscriber sub;
    queue.addSubscriber(&sub);

    auto start = std::chrono::high_resolution_clock::now();

    CompressedReader reader(argv[1], &queue);

    while (queue.nextEvent())
    {
    }

    auto end = std::chrono::high_resolution_clock::now();
    double seconds = std::chrono::duration<double>(end - start).count();

    std::cout << std::endl;
    std::cout << "Finished pipeline run" << std::endl;
    std::cout << "Total events: " << sub.count << std::endl;
    std::cout << "Elapsed: " << seconds << " sec" << std::endl;

    if (seconds > 0.0)
    {
        std::cout << "Throughput: " << (double)sub.count / seconds << " events/sec" << std::endl;
    }

    return 0;
}
