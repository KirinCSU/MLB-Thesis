#include <iostream>
#include <cstdlib>
#include <chrono>

#include "Books.h"
#include "CompressedReader.h"
#include "DailyBars.h"
#include "EventQueue.h"
#include "SnapshotWriter.h"

#include <filesystem>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace MLB;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <compressed_file> <daily_bars_file> <N>" << std::endl;
        return 1;
    }

    //Args
    int n = atoi(argv[3]);
    DailyBars bars(argv[2]);
    Strings tickers = bars.topNLiquid(n);

    std::cout << "Loaded daily bars: " << bars.size() << std::endl;
    std::cout << "Using top " << tickers.size() << " tickers" << std::endl;

    //Objects
    EventQueue queue;
    Books books;
    queue.addSubscriber(&books);

    //Timer
    auto start = std::chrono::high_resolution_clock::now();

    CompressedReader reader(argv[1], &queue, tickers);

    //Dir Creation
    std::time_t now = std::time(nullptr);
    std::tm localTime = *std::localtime(&now);

    std::ostringstream dateStream;
    dateStream << std::put_time(&localTime, "%Y-%m-%d");

    String location = "/home/sudhir/kirin/Features/";
    String runDir = location + std::to_string(n) + "run." + dateStream.str();

    std::filesystem::create_directories(runDir);

    std::cout << "Run directory: " << runDir << std::endl;

    //File Setup
    String eventTicker;
    std::map<String, bool> linked;
    SnapshotWriter writer(&books, tickers, 3);

    for(const String& ticker : tickers)
    {
        linked[ticker] = false;
    }

    Event* event;
    BookEvent* bookEvent;
    Time eventTime;

    Time open("09:30:00.000000");
    Time five("00:05:00.000000");
    Time first = open + five;
    Time close("16:00:00.000000");

    //Loop
    while (queue.haveEvent())
    {
        event = queue.peakEvent();
        if(event->eventType == Event::Type::Book)
        {
            bookEvent = dynamic_cast<BookEvent*>(event);

            if(bookEvent != nullptr)
            {
                eventTicker = bookEvent->spec->ticker;
                eventTime = bookEvent->eventTime;

                if(eventTime >= open)
                {
                    open += five;

                    if(eventTime >= first)
                    {
                        std::cout << "Time to Update & Write - " << eventTime.str() << std::endl;
                        writer.update();
                        writer.write();
                    }
                    else
                    {
                        std::cout << "Time to Update         - " << eventTime.str() << std::endl;
                        writer.update();
                    }
                }

                if(linked[eventTicker] == false)
                {
                    writer.link(bookEvent->spec);
                    linked[eventTicker] = true;
                }
            }
        }

        queue.nextEvent();
    }

    //Metrics
    auto end = std::chrono::high_resolution_clock::now();
    double seconds = std::chrono::duration<double>(end - start).count();

    std::cout << "Finished top-N full pipeline run" << std::endl;
    std::cout << "Books created: " << books.size() << std::endl;
    std::cout << "Elapsed: " << seconds << " sec" << std::endl;

    Int64 events = queue.getTotalEvents();

    std::cout << "Events: " << events << std::endl;

    if (seconds > 0.0)
    {
        std::cout << "Throughput: " << events / seconds << " events/sec" << std::endl;
    }
    
    return 0;
}
