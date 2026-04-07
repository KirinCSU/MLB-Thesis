#include "Compressed.h"

namespace MLB
{
    namespace Compressed
    {

        void SetTimeStamp(Entry& entry, int secFromMid)
        {
            entry.subEntryA = (100 * TimeStamp) << 20;
            entry.subEntryB = secFromMid;
            entry.subEntryC = 0;
        }

        void SetNewTicker(Entry& entry, int tickerID, const String& ticker)
        {
            if (tickerID >= 100000)
            {
                ERROR("TickerID must be less than 100000: " + Itos(tickerID) + " " + ticker);
            }

            String copy = ticker;

            if (copy.size() > 6)
            {
                copy = copy.substr(0, 6);
            }

            while (copy.size() < 6)
            {
                copy += ' ';
            }

            entry.subEntryA = (100 * NewTicker + tickerID / 1000) << 20;
            entry.subEntryB = (tickerID % 1000) << 20;
            entry.subEntryC = 0;

            entry.subEntryB |= copy[0];
            entry.subEntryB |= copy[1] << 8;

            entry.subEntryC |= copy[2];
            entry.subEntryC |= copy[3] << 8;
            entry.subEntryC |= copy[4] << 16;
            entry.subEntryC |= copy[5] << 24;
        }

        void SetNewPrice(Entry& entry, int tickerID, int price)
        {
            entry.subEntryA = (100 * NewPrice + tickerID / 1000) << 20;
            entry.subEntryB = (tickerID % 1000) << 20;
            entry.subEntryC = price;
        }


        void SetAdditionBuy(Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset)
        {
            CheckSize(AdditionBuy, size);
            CheckPriceOffset(AdditionBuy, priceOffset);

            priceOffset += 8;
            int poDigit1 = priceOffset / 4;
            int poDigit2 = priceOffset % 4;

            entry.subEntryA = (poDigit1 * 1000 + 100 * AdditionBuy + tickerID / 1000) << 20;
            entry.subEntryA |= usec;

            entry.subEntryB = (poDigit2 * 1000 + tickerID % 1000) << 20;
            entry.subEntryB |= size;

            entry.subEntryC = orn;
        }

        void SetAdditionSell(Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset)
        {
            CheckSize(AdditionSell, size);
            CheckPriceOffset(AdditionSell, priceOffset);

            priceOffset += 8;
            int poDigit1 = priceOffset / 4;
            int poDigit2 = priceOffset % 4;

            entry.subEntryA = (poDigit1 * 1000 + 100 * AdditionSell + tickerID / 1000) << 20;
            entry.subEntryA |= usec;

            entry.subEntryB = (poDigit2 * 1000 + tickerID % 1000) << 20;
            entry.subEntryB |= size;

            entry.subEntryC = orn;
        }

        void SetDeletion (Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size)
        {
            CheckSize(Deletion, size);

            entry.subEntryA = (100 * Deletion + tickerID / 1000) << 20;
            entry.subEntryA |= usec;

            entry.subEntryB = (tickerID % 1000) << 20;
            entry.subEntryB |= size;

            entry.subEntryC = orn;
        }

        void SetExecution(Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size)
        {
            CheckSize(Deletion, size);

            entry.subEntryA = (100 * Execution + tickerID / 1000) << 20;
            entry.subEntryA |= usec;

            entry.subEntryB = (tickerID % 1000) << 20;
            entry.subEntryB |= size;

            entry.subEntryC = orn;
        }

        void SetRevision(Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset)
        {
            CheckSize(Revision, size);
            CheckPriceOffset(Revision, priceOffset);

            priceOffset += 8;
            int poDigit1 = priceOffset / 4;
            int poDigit2 = priceOffset % 4;

            entry.subEntryA = (poDigit1 * 1000 + 100 * Revision + tickerID / 1000) << 20;
            entry.subEntryA |= usec;

            entry.subEntryB = (poDigit2 * 1000 + tickerID % 1000) << 20;
            entry.subEntryB |= size;

            entry.subEntryC = orn;
        }

        void SetTradeBuy(Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset)
        {
            CheckSize(TradeBuy, size);
            CheckPriceOffset(TradeBuy, priceOffset);

            priceOffset += 8;
            int poDigit1 = priceOffset / 4;
            int poDigit2 = priceOffset % 4;

            entry.subEntryA = (poDigit1 * 1000 + 100 * TradeBuy + tickerID / 1000) << 20;
            entry.subEntryA |= usec;

            entry.subEntryB = (poDigit2 * 1000 + tickerID % 1000) << 20;
            entry.subEntryB |= size;

            entry.subEntryC = orn;
        }

        void SetTradeSell(Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset)
        {
            CheckSize(TradeSell, size);
            CheckPriceOffset(TradeSell, priceOffset);

            priceOffset += 8;
            int poDigit1 = priceOffset / 4;
            int poDigit2 = priceOffset % 4;

            entry.subEntryA = (poDigit1 * 1000 + 100 * TradeSell + tickerID / 1000) << 20;
            entry.subEntryA |= usec;

            entry.subEntryB = (poDigit2 * 1000 + tickerID % 1000) << 20;
            entry.subEntryB |= size;

            entry.subEntryC = orn;
        }

        Type GetType(const Entry& entry)
        {
            return (Type)(((entry.subEntryA >> 20) / 100) % 10);
        }

        int GetTickerID(const Entry& entry)
        {
            return (entry.subEntryB >> 20) % 1000 + 1000 * ((entry.subEntryA >> 20) % 100);
        }

        int GetSecFromMid(const Entry& entry)
        {
            return entry.subEntryB;
        }

        int GetUSec(const Entry& entry)
        {
            return ((entry.subEntryA << 12) >> 12);
        }

        int GetSize(const Entry& entry)
        {
            return ((entry.subEntryB << 12) >> 12);
        }

        int GetPriceOffset(const Entry& entry)
        {
            return 4 * ((entry.subEntryA >> 20) / 1000) + ((entry.subEntryB >> 20) / 1000) - 8;
        }

        OrderReferenceNumber GetORN(const Entry& entry)
        {
            return entry.subEntryC;
        }

        int GetNewPrice(const Entry& entry)
        {
            return entry.subEntryC;
        }

        String GetNewTicker(const Entry& entry)
        {
            char ticker[6];
            ticker[0] = (entry.subEntryB)       & 0xFF;
            ticker[1] = (entry.subEntryB >> 8)  & 0xFF;

            ticker[2] = (entry.subEntryC)       & 0xFF;
            ticker[3] = (entry.subEntryC >> 8)  & 0xFF;
            ticker[4] = (entry.subEntryC >> 16) & 0xFF;
            ticker[5] = (entry.subEntryC >> 24) & 0xFF;

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
                case NewTicker :
                    return "NewTicker";
                case NewPrice :
                    return "NewPrice";
                case AdditionBuy :
                    return "AdditionBuy";
                case AdditionSell :
                    return "AdditionSell";
                case Deletion :
                    return "Deletion";
                case Execution :
                    return "Execution";
                case Revision :
                    return "Revision";
                case TradeBuy :
                    return "TradeBuy";
                case TradeSell :
                    return "TradeSell";
                default :
                    break;
            }

            ERROR("Unknown Level03 Type: " + Itos(type));
            return "Unknown";
        }

        void CheckSize(Type type, int size)
        {
            if (size >= 1000000)
            {
                ERROR("Size too large (must be less than 1,000,000): " + Itos(size) + " in " + TypeToStr(type));
            }
        }

        void CheckPriceOffset(Type type, int priceOffset)
        {
            if (priceOffset > 7 || priceOffset < -8)
            {
                ERROR("PriceOffset out of range (-8 <= po <= 7): " + Itos(priceOffset) + " in " + TypeToStr(type));
            }
        }

    }
}
