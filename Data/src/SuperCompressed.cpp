#include "SuperCompressed.h"

namespace MLB
{
    namespace SuperCompressed
    {
        void SetTimeStamp(Record& record, int secFromMid)
        {
            record.blockA = (100 * TimeStamp) << 20;
            record.blockB = secFromMid;
        }

        void SetTicker(Record& record, const String& ticker)
        {
            if (ticker.size() > 6)
            {
                ERROR("Ticker is too long (max size 6 char): " + ticker);
            }

            String copy = ticker;
            while (copy.size() < 6)
            {
                copy += ' ';
            }

            record.blockA = (100 * Ticker) << 20;
            record.blockA |= copy[0];
            record.blockA |= copy[1] << 8;

            record.blockB = 0;
            record.blockB |= copy[2];
            record.blockB |= copy[3] << 8;
            record.blockB |= copy[4] << 16;
            record.blockB |= copy[5] << 24;
        }

        void SetNewTickerID(Record& record, int tickerID)
        {
            if (tickerID >= 100000)
            {
                ERROR("TickerID must be less than 100000: " + Itos(tickerID));
            }

            record.blockA = (100 * NewTickerID + tickerID / 1000) << 20;
            record.blockB = (tickerID % 1000) << 22;
        }

        void SetPrice(Record& record, int price)
        {
            record.blockA = (100 * Price) << 20;
            record.blockB = price;
        }

        void SetExecInsertTime(Record& record, int sec, int usec)
        {
            record.blockA = (100 * ExecInsertTime) << 20;
            record.blockA |= sec;
            record.blockB = usec;
        }

        void SetData(Record& record, Type type, int usec, int tickerID, UInt32 data)
        {
            ASSERT(data < 4194304);

            record.blockA  = (100 * type + tickerID / 1000) << 20;
            record.blockA |= usec;

            record.blockB  = (tickerID % 1000) << 22;
            record.blockB |= data;
        }

        Type GetType(const Record& record)
        {
            return (Type)((record.blockA >> 20) / 100);
        }

        int GetTickerID(const Record& record)
        {
            return (record.blockB >> 22) % 1000 + 1000 * ((record.blockA >> 20) % 100);
        }

        int GetSecFromMid(const Record& record)
        {
            return record.blockB;
        }

        int GetUSec(const Record& record)
        {
            return ((record.blockA << 12) >> 12);
        }

        int GetPrice(const Record& record)
        {
            return record.blockB;
        }

        int GetInsertSec(const Record& record)
        {
            return (record.blockA << 12) >> 12;
        }

        int GetInsertUSec(const Record& record)
        {
            return record.blockB;
        }

        int GetData(const Record& record)
        {
            return ((record.blockB << 10) >> 10);
        }

        String GetTicker(const Record& record)
        {
            char ticker[6];
            ticker[0] = (record.blockA)       & 0xFF;
            ticker[1] = (record.blockA >> 8)  & 0xFF;

            ticker[2] = (record.blockB)       & 0xFF;
            ticker[3] = (record.blockB >> 8)  & 0xFF;
            ticker[4] = (record.blockB >> 16) & 0xFF;
            ticker[5] = (record.blockB >> 24) & 0xFF;

            int length = 5;
            while (length >= 0 && ticker[length] == ' ')
            {
                length--;
            }

            if (length < 0)
            {
                return String();
            }

            return String(ticker, length + 1);
        }

        String TypeToStr(Type type)
        {
            switch (type)
            {
                case TimeStamp :
                    return "TimeStamp";
                case Ticker :
                    return "Ticker";
                case NewTickerID :
                    return "NewTickerID";
                case BidSizeChange :
                    return "BidSizeChange";
                case AskSizeChange :
                    return "AskSizeChange";
                case BidPriceChange :
                    return "BidPriceChange";
                case AskPriceChange :
                    return "AskPriceChange";
                case ExecInsertTime :
                    return "ExecInsertTime";
                case BidExecution :
                    return "BidExecution";
                case AskExecution :
                    return "AskExecution";
                case Price :
                    return "Price";
                case HiddenTradeBuy :
                    return "HiddenTradeBuy";
                case HiddenTradeSell :
                    return "HiddenTradeSell";
                default :
                    break;
            }

            ERROR("Unknown Level01 Type: " + Itos(type));
            return "Unknown";
        }
    }
}
