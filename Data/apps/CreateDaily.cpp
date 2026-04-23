#include <iostream>
#include <fstream>
#include <map>

#include "CompressedReader.h"
#include "EventQueue.h"
#include "Subscriber.h"
#include "BookEvent.h"
#include "Spec.h"

using namespace MLB;

struct DailyStats
{
    bool haveTrade;

    double open;
    double high;
    double low;
    double close;

    Int64 volume;
    double notional;

    DailyStats()
        : haveTrade(false),
          open(0.0),
          high(0.0),
          low(0.0),
          close(0.0),
          volume(0),
          notional(0.0)
    {
    }

    void onTrade(double price, int size)
    {
        if (!haveTrade)
        {
            haveTrade = true;
            open = price;
            high = price;
            low = price;
            close = price;
        }
        else
        {
            if (price > high)
            {
                high = price;
            }

            if (price < low)
            {
                low = price;
            }

            close = price;
        }

        volume += size;
        notional += price * size;
    }

    double vwap() const
    {
        if (volume <= 0)
        {
            return 0.0;
        }

        return notional / volume;
    }
};

class DailyBarSubscriber : public Subscriber
{
private:
    std::map<String, DailyStats> statsByTicker;

public:
    DailyBarSubscriber()
    {
    }

    void onEvent(const BookEvent* event)
    {
        if (!event->isHidTrade())
        {
            return;
        }

        if (!event->spec)
        {
            return;
        }

        statsByTicker[event->spec->ticker].onTrade(event->price(), event->size());
    }

    void writeFile(const char* filename) const
    {
        std::ofstream out(filename);

        if (!out)
        {
            std::cout << "Unable to open output file: " << filename << std::endl;
            return;
        }

        out << "TICKER OPEN HIGH LOW CLOSE VOLUME VWAP" << std::endl;

        std::map<String, DailyStats>::const_iterator it = statsByTicker.begin();
        while (it != statsByTicker.end())
        {
            const String& ticker = it->first;
            const DailyStats& stats = it->second;

            if (stats.haveTrade)
            {
                out << ticker << " "
                    << stats.open << " "
                    << stats.high << " "
                    << stats.low << " "
                    << stats.close << " "
                    << stats.volume << " "
                    << stats.vwap() << std::endl;
            }

            ++it;
        }
    }

    int size() const
    {
        return statsByTicker.size();
    }
};

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <compressed_file> <output_file>" << std::endl;
        return 1;
    }

    EventQueue queue;
    DailyBarSubscriber sub;
    queue.addSubscriber(&sub);

    CompressedReader reader(argv[1], &queue);

    while (queue.nextEvent())
    {
    }

    sub.writeFile(argv[2]);

    std::cout << "Wrote daily bars for " << sub.size() << " tickers" << std::endl;

    return 0;
}
