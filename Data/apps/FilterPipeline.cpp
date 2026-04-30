#include <iostream>
#include <cstdlib>
#include <chrono>

#include "Books.h"
#include "CompressedReader.h"
#include "DailyBars.h"
#include "EventQueue.h"

using namespace MLB;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <compressed_file> <daily_bars_file> <N>" << std::endl;
        return 1;
    }

    int n = atoi(argv[3]);

    DailyBars bars(argv[2]);
    Strings tickers = bars.topNLiquid(n);

    std::cout << "Loaded daily bars: " << bars.size() << std::endl;
    std::cout << "Using top " << tickers.size() << " tickers" << std::endl;

    EventQueue queue;
    Books books;
    queue.addSubscriber(&books);

    auto start = std::chrono::high_resolution_clock::now();

    CompressedReader reader(argv[1], &queue, tickers);

    while (queue.nextEvent())
    {
    }

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
