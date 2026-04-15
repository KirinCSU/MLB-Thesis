#include "CompressedReader.h"

MLB::CompressedReader::CompressedReader(const char* filename, EventQueue* a_eventQueue)
    : seconds(0),
      index(0),
      maxIndex(0),
      head(0),
      tail(0),
      eventQueue(a_eventQueue),
      file(0)
{
    file = fopen(filename, "rb");
    if (!file)
    {
        ERROR(String("Unable to open file: ") + filename);
    }

    nextEvent();
}

MLB::CompressedReader::~CompressedReader()
{
    if (file)
    {
        fclose(file);
        file = 0;
    }
}

void MLB::CompressedReader::nextEvent()
{
    head = 0;
    tail = 0;
    getNextRecord();

    if (head)
    {
        eventQueue->insertEvent(head);
    }
}

void MLB::CompressedReader::getNextRecord()
{
    if (!file)
    {
        return;
    }

    bool first = true;
    int currUSec = 0;
    Time eventTime;

    while (true)
    {
        if (index >= maxIndex)
        {
            index = 0;
            maxIndex = fread(buffer, sizeof(Compressed::Entry), MaxEntries, file);
        }

        if (maxIndex == 0)
        {
            fclose(file);
            file = 0;
            return;
        }

        while (index < maxIndex)
        {
            Compressed::Entry& entry = buffer[index];
            Compressed::Type msgType = Compressed::GetType(entry);
            int tickerID = Compressed::GetTickerID(entry);

            if (msgType <= Compressed::NewPrice)
            {
                switch (msgType)
                {
                case Compressed::TimeStamp :
                {
                    if (!first)
                    {
                        return;
                    }

                    seconds = Compressed::GetSecFromMid(entry);
                    break;
                }

                case Compressed::NewTicker :
                {
                    String ticker = Compressed::GetNewTicker(entry);

                    while ((int)prices.size() <= tickerID)
                    {
                        prices.push_back(0);
                        tickerIDToSpec.push_back(0);
                    }

                    if (!tickerIDToSpec[tickerID])
                    {
                        tickerIDToSpec[tickerID] = new Spec(ticker, tickerID);
                    }

                    break;
                }

                case Compressed::NewPrice :
                {
                    while ((int)prices.size() <= tickerID)
                    {
                        prices.push_back(0);
                        tickerIDToSpec.push_back(0);
                    }

                    prices[tickerID] = Compressed::GetNewPrice(entry);
                    break;
                }

                default :
                    break;
                }

                index++;
                continue;
            }

            const Spec* spec = 0;

            if (tickerID >= 0 && tickerID < (int)tickerIDToSpec.size())
            {
                spec = tickerIDToSpec[tickerID];
            }

            if (!spec)
            {
                index++;
                continue;
            }

            int usec = Compressed::GetUSec(entry);

            if (first)
            {
                currUSec = usec;
                eventTime.sec = seconds;
                eventTime.usec = usec;
                first = false;
            }
            else if (usec != currUSec)
            {
                return;
            }

            BookEvent* event = new BookEvent(this, eventTime);

            switch (msgType)
            {
            case Compressed::AdditionBuy :
            {
                double price = (prices[tickerID] + Compressed::GetPriceOffset(entry) * 100) / 10000.0;
                event->setAddition(spec,
                                   Compressed::GetORN(entry),
                                   Buy,
                                   Compressed::GetSize(entry),
                                   price);
                break;
            }

            case Compressed::AdditionSell :
            {
                double price = (prices[tickerID] + Compressed::GetPriceOffset(entry) * 100) / 10000.0;
                event->setAddition(spec,
                                   Compressed::GetORN(entry),
                                   Sell,
                                   Compressed::GetSize(entry),
                                   price);
                break;
            }

            case Compressed::Deletion :
            {
                event->setDeletion(spec,
                                   Compressed::GetORN(entry),
                                   Compressed::GetSize(entry));
                break;
            }

            case Compressed::Execution :
            {
                event->setExecution(spec,
                                    Compressed::GetORN(entry),
                                    Compressed::GetSize(entry));
                break;
            }

            case Compressed::Revision :
            {
                double price = (prices[tickerID] + Compressed::GetPriceOffset(entry) * 100) / 10000.0;
                event->setRevision(spec,
                                   Compressed::GetORN(entry),
                                   Compressed::GetSize(entry),
                                   price);
                break;
            }

            case Compressed::TradeBuy :
            {
                double price = (prices[tickerID] + Compressed::GetPriceOffset(entry) * 100) / 10000.0;
                event->setHidTrade(spec,
                                   Compressed::GetORN(entry),
                                   Buy,
                                   Compressed::GetSize(entry),
                                   price);
                break;
            }

            case Compressed::TradeSell :
            {
                double price = (prices[tickerID] + Compressed::GetPriceOffset(entry) * 100) / 10000.0;
                event->setHidTrade(spec,
                                   Compressed::GetORN(entry),
                                   Sell,
                                   Compressed::GetSize(entry),
                                   price);
                break;
            }

            default :
                break;
            }

            if (!head)
            {
                head = event;
                tail = event;
            }
            else
            {
                tail = (BookEvent*)tail->link(event);
            }

            index++;
        }
    }
}
