#include <iostream>

#include "Books.h"
#include "CompressedReader.h"
#include "EventQueue.h"
#include "Subscriber.h"
#include "BookEvent.h"

using namespace MLB;

class OneTickerSubscriber : public Subscriber
{
private:
    String ticker;
    Books* books;
    Int64 count;

public:
    OneTickerSubscriber(const String& a_ticker, Books* a_books)
        : ticker(a_ticker),
          books(a_books),
          count(0)
    {
    }

    void onEvent(const BookEvent* event)
    {
        if (!event->spec)
        {
            return;
        }

        if (event->spec->ticker != ticker)
        {
            return;
        }

        count++;

        if (count == 1)
        {
            std::cout << "First event seen for " << ticker
                      << " at " << event->eventTime.str(true) << std::endl;
        }

        if (count % 100000 == 0)
        {
            std::cout << ticker << " events processed: " << count
                      << " at " << event->eventTime.str(true) << std::endl;
        }

        books->onEvent(event);
    }

    Int64 size() const
    {
        return count;
    }
};

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <compressed_file> <ticker>" << std::endl;
        return 1;
    }

    EventQueue queue;
    Books books;
    OneTickerSubscriber sub(argv[2], &books);

    queue.addSubscriber(&sub);

    CompressedReader reader(argv[1], &queue);

    while (queue.nextEvent())
    {
    }

    std::cout << "Finished one-ticker pipeline run" << std::endl;
    std::cout << "Ticker: " << argv[2] << std::endl;
    std::cout << "Events processed: " << sub.size() << std::endl;
    std::cout << "Books created: " << books.size() << std::endl;

    return 0;
}
