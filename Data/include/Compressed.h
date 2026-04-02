#ifndef COMPRESSED_H
#define COMPRESSED_H

#include "OrderReferenceNumber.h"

namespace MLB
{

    namespace Compressed
    {
        enum Type
        {
            TimeStamp = 0,
            NewTicker = 1,
            NewPrice = 2,
            AdditionBuy = 3,
            AdditionSell = 4,
            Deletion = 5,
            Execution = 6,
            Revision = 7,
            TradeBuy = 8,
            TradeSell = 9
        };

        struct Entry
        {
            UInt32 subEntryA;
            UInt32 subEntryB;
            UInt32 subEntryC;
        };

        void SetTimeStamp(Entry& entry, int msec);
        void SetNewTicker(Entry& entry, int tickerID, const String& ticker);
        void SetNewPrice (Entry& entry, int tickerID, int price);

        void SetAdditionBuy( Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset);
        void SetAdditionSell(Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset);

        void SetDeletion (Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size);
        void SetExecution(Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size);
        void SetRevision (Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset);
        void SetTradeBuy (Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset);
        void SetTradeSell(Entry& entry, int usec, int tickerID, OrderReferenceNumber orn, int size, int priceOffset);

        Type GetType(const Entry& entry);
        int  GetTickerID(const Entry& entry);
        int  GetSecFromMid(const Entry& entry);
        int  GetUSec(const Entry& entry);
        int  GetSize(const Entry& entry);
        int  GetPriceOffset(const Entry& entry);
        int  GetNewPrice(const Entry& entry);
        OrderReferenceNumber  GetORN(const Entry& entry);
        String GetNewTicker(const Entry& entry);

        String TypeToStr(Type type);
        void CheckSize(Type type, int size);
        void CheckPriceOffset(Type type, int priceOffset);
    }

}

#endif
