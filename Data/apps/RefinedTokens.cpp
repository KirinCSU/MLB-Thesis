
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>

#include "Books.h"
#include "CompressedReader.h"
#include "DailyBars.h"
#include "EventQueue.h"

#include "BookEvent.h"
#include "Tokens.h"

#include "Loaders.h"

using namespace MLB;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <compressed_file> <daily_bars_file> <bod_file>" << std::endl;
        return 1;
   }

    //SETUP

    //Ticker Setup
    DailyBars bars(argv[2]);
    Strings tickers = bars.topNLiquid(1);

    if (tickers.empty())
    {
        std::cerr << "No tickers found" << std::endl;
        return 1;
    }

    std::cout << "Selected ticker: " << tickers[0] << std::endl;

    //Vol Setup
    Vols vols = loadVols(argv[3]);

    //Mid Setup
    std::map<String, double> mids;

    for(const auto& ticker : tickers)
    {
        mids[ticker] = 0.0;
    }

    // OBJECTS

    EventQueue queue;
    Books books;
    queue.addSubscriber(&books);

    CompressedReader reader(argv[1], &queue, tickers);

    Event* event;
    BookEvent* bookEvent;

    // Main Loop
    while (queue.haveEvent())
    {
        event = queue.peakEvent();

        if(event->eventType == Event::Type::Book)
        {
            bookEvent = dynamic_cast<BookEvent*>(event);

            Book* book = books.get(bookEvent->spec);
            if(book == nullptr)
            {
                queue.nextEvent();
                continue;
            }

            Token token{};

            // Populate State and Event Fields
            FillState(&token, bookEvent, &books, vols);

            // Send Event for Processing
            //std::cout << book->bestBid() << " " << book->bestAsk() << std::endl;
            queue.nextEvent();
            //std::cout << book->bestBid() << " " << book->bestAsk() << std::endl;

            //Populate Change Fields
            FillChange(&token, book, vols);

            PrintToken(token);
        }
        else
        {
            queue.nextEvent();
        }
    }

    std::cout << "Done" << std::endl;

    return 0;
}

