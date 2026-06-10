#ifndef SUPER_COMPRESSED_H
#define SUPER_COMPRESSED_H

namespace MLB
{
    namespace SuperCompressed
    {
        enum Type
        {
            TimeStamp = 0,
            Ticker = 1,
            NewTickerID = 2,
            Price = 3,
            ExecInsertTime = 4,
            BidSizeChange = 5,
            AskSizeChange = 6,
            BidPriceChange = 7,
            AskPriceChange = 8,
            BidExecution = 9,
            AskExecution = 10,
            HiddenTradeBuy = 11,
            HiddenTradeSell = 12
        };

        struct Record
        {
            UInt32 blockA;
            UInt32 blockB;
        };

        // Modifiers
        void SetTimeStamp(Record& record, int sec);
        void SetTicker(Record& record, const String& ticker);
        void SetNewTickerID(Record& record, int tickerID);
        void SetPrice(Record& record, int price);
        void SetExecInsertTime(Record& record, int sec, int usec);
        void SetData(Record& record, Type type, int usec, int tickerID, UInt32 data);

        // Accessors
        Type GetType(const Record& record);
        int  GetTickerID(const Record& record);
        int  GetSecFromMid(const Record& record);
        int  GetUSec(const Record& record);
        int  GetPrice(const Record& record);
        int  GetInsertSec(const Record& record);
        int  GetInsertUSec(const Record& record);
        int  GetData(const Record& record);
        String GetTicker(const Record& record);

        // Helpers
        String TypeToStr(Type type);
    }

}

#endif
