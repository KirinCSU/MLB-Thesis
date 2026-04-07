#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

#include "ItchBase.h"
#include "Elements.h"
#include "Helpers.h"
#include "OrderReferenceNumber.h"
#include "Time.h"
#include "Compressed.h"

using namespace MLB;

class Converter : public Itch::StreamCB
{
    private:
        typedef std::map<OrderReferenceNumber, OrderReferenceNumber> Revisions;
        typedef std::map<OrderReferenceNumber, PairII> ORNToInfo;

    private:
        FILE* output;
        Compressed::Entry entry;

        Ints prices;
        MapSI tickerIDs;

        ORNToInfo ornToInfo;
        Revisions revisions;

        UInt32 prevSecFromMidnight;

    public:
        Converter(FILE* a_output)
            : output(a_output),
              prevSecFromMidnight(0)
        {
        }

        void onAddOrderMsg(const Itch::AddOrder* msg)
        {
            int tickerID = getTickerID(String(msg->symbol, Itch::Length(msg->symbol)));
            UInt32 usec = checkNeedFromTimeStampEntry(msg->timeStamp);

            if (CharToBuy(msg->side))
            {
                Compressed::SetAdditionBuy(entry,
                                           usec,
                                           tickerID,
                                           GetUInt64(msg->orn),
                                           GetUInt32(msg->shares),
                                           getOffset(tickerID, GetUInt32(msg->price)));
            }
            else
            {
                Compressed::SetAdditionSell(entry,
                                            usec,
                                            tickerID,
                                            GetUInt64(msg->orn),
                                            GetUInt32(msg->shares),
                                            getOffset(tickerID, GetUInt32(msg->price)));
            }

            ornToInfo[GetUInt64(msg->orn)] = PairII(tickerID, GetUInt32(msg->shares));
            writeEntry();
        }

        void onAddOrderWithMPIDMsg(const Itch::AddOrderWithMPID* msg)
        {
            int tickerID = getTickerID(String(msg->symbol, Itch::Length(msg->symbol)));
            UInt32 usec = checkNeedFromTimeStampEntry(msg->timeStamp);

            if (CharToBuy(msg->side))
            {
                Compressed::SetAdditionBuy(entry,
                                           usec,
                                           tickerID,
                                           GetUInt64(msg->orn),
                                           GetUInt32(msg->shares),
                                           getOffset(tickerID, GetUInt32(msg->price)));
            }
            else
            {
                Compressed::SetAdditionSell(entry,
                                            usec,
                                            tickerID,
                                            GetUInt64(msg->orn),
                                            GetUInt32(msg->shares),
                                            getOffset(tickerID, GetUInt32(msg->price)));
            }

            writeEntry();
            ornToInfo[GetUInt64(msg->orn)] = PairII(tickerID, GetUInt32(msg->shares));
        }

        void onOrderExecutedMsg(const Itch::OrderExecuted* msg)
        {
            UInt32 usec = checkNeedFromTimeStampEntry(msg->timeStamp);

            OrderReferenceNumber orn = getOrn(GetUInt64(msg->orn));
            ORNToInfo::iterator find = ornToInfo.find(orn);

            if (find != ornToInfo.end())
            {
                Compressed::SetExecution(entry,
                                         usec,
                                         find->second.first,
                                         orn,
                                         GetUInt32(msg->execShares));

                writeEntry();

                find->second.second -= GetUInt32(msg->execShares);
                if (find->second.second <= 0)
                {
                    ornToInfo.erase(find);
                    revisions.erase(GetUInt64(msg->orn));
                }
            }
        }

        void onOrderExecutedWithPriceMsg(const Itch::OrderExecutedWithPrice* msg)
        {
            UInt32 usec = checkNeedFromTimeStampEntry(msg->timeStamp);

            OrderReferenceNumber orn = getOrn(GetUInt64(msg->orn));
            ORNToInfo::iterator find = ornToInfo.find(orn);

            if (find != ornToInfo.end())
            {
                Compressed::SetDeletion(entry,
                                        usec,
                                        find->second.first,
                                        orn,
                                        GetUInt32(msg->execShares));

                writeEntry();

                find->second.second -= GetUInt32(msg->execShares);
                if (find->second.second <= 0)
                {
                    ornToInfo.erase(find);
                    revisions.erase(GetUInt64(msg->orn));
                }
            }
        }

        void onOrderCancelMsg(const Itch::OrderCancel* msg)
        {
            UInt32 usec = checkNeedFromTimeStampEntry(msg->timeStamp);

            OrderReferenceNumber orn = getOrn(GetUInt64(msg->orn));
            ORNToInfo::iterator find = ornToInfo.find(orn);

            if (find != ornToInfo.end())
            {
                Compressed::SetDeletion(entry,
                                        usec,
                                        find->second.first,
                                        orn,
                                        GetUInt32(msg->cxldShares));

                writeEntry();

                find->second.second -= GetUInt32(msg->cxldShares);
                if (find->second.second <= 0)
                {
                    ornToInfo.erase(find);
                    revisions.erase(GetUInt64(msg->orn));
                }
            }
        }

        void onOrderDeleteMsg(const Itch::OrderDelete* msg)
        {
            UInt32 usec = checkNeedFromTimeStampEntry(msg->timeStamp);

            OrderReferenceNumber orn = getOrn(GetUInt64(msg->orn));
            ORNToInfo::iterator find = ornToInfo.find(orn);

            if (find != ornToInfo.end())
            {
                Compressed::SetDeletion(entry,
                                        usec,
                                        find->second.first,
                                        orn,
                                        0);

                writeEntry();

                ornToInfo.erase(find);
                revisions.erase(GetUInt64(msg->orn));
            }
        }

        void onOrderReplaceMsg(const Itch::OrderReplace* msg)
        {
            UInt32 usec = checkNeedFromTimeStampEntry(msg->timeStamp);

            OrderReferenceNumber orn = getOrn(GetUInt64(msg->origOrn));
            ORNToInfo::iterator find = ornToInfo.find(orn);

            if (find != ornToInfo.end())
            {
                Compressed::SetRevision(entry,
                                        usec,
                                        find->second.first,
                                        orn,
                                        GetUInt32(msg->newShares),
                                        getOffset(find->second.first, GetUInt32(msg->newPrice)));

                writeEntry();

                revisions.erase(GetUInt64(msg->origOrn));
                revisions[GetUInt64(msg->newOrn)] = orn;
                find->second.second = GetUInt32(msg->newShares);
            }
        }

        void onTradeMsg(const Itch::Trade* msg)
        {
            UInt32 usec = checkNeedFromTimeStampEntry(msg->timeStamp);
            int tickerID = getTickerID(String(msg->symbol, Itch::Length(msg->symbol)));

            if (CharToBuy(msg->side))
            {
                Compressed::SetTradeBuy(entry,
                                        usec,
                                        tickerID,
                                        GetUInt64(msg->orn),
                                        GetUInt32(msg->shares),
                                        getOffset(tickerID, GetUInt32(msg->price)));
            }
            else
            {
                Compressed::SetTradeSell(entry,
                                         usec,
                                         tickerID,
                                         GetUInt64(msg->orn),
                                         GetUInt32(msg->shares),
                                         getOffset(tickerID, GetUInt32(msg->price)));
            }

            writeEntry();
        }

        int getTickerID(const String& ticker)
        {
            MapSI::iterator find = tickerIDs.find(ticker);
            if (find != tickerIDs.end())
            {
                return find->second;
            }

            int tickerID = tickerIDs.size();
            tickerIDs[ticker] = tickerID;
            prices.push_back(0);

            Compressed::SetNewTicker(entry, tickerID, ticker);
            writeEntry();

            return tickerID;
        }

        int getOffset(int tickerID, int price)
        {
            int diff = price - prices[tickerID];

            if (diff % 100 != 0 || diff > 700 || diff < -800)
            {
                prices[tickerID] = price;
                Compressed::SetNewPrice(entry, tickerID, price);
                writeEntry();
                return 0;
            }

            return diff / 100;
        }

        OrderReferenceNumber getOrn(OrderReferenceNumber orn)
        {
            Revisions::iterator find = revisions.find(orn);
            if (find != revisions.end())
            {
                return find->second;
            }

            return orn;
        }

        UInt32 checkNeedFromTimeStampEntry(const char* timeStamp)
        {
            UInt64 nsecFromMidnight = Itch::getNsecFromMidnight(timeStamp);
            UInt64 usecFromMidnight = nsecFromMidnight / 1000;

            UInt32 usec = usecFromMidnight % 1000000;
            UInt32 secFromMidnight = (usecFromMidnight - usec) / 1000000;

            if (prevSecFromMidnight != secFromMidnight)
            {
                Compressed::SetTimeStamp(entry, secFromMidnight);
                writeEntry();
            }

            prevSecFromMidnight = secFromMidnight;
            return usec;
        }

        void writeEntry()
        {
            fwrite(&entry, sizeof(Compressed::Entry), 1, output);
        }
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Needs OutputFile" << std::endl;
        return 1;
    }

    FILE* output = fopen(argv[1], "wb");
    if (!output)
    {
        std::cerr << "Unable to open output file: " << argv[1] << std::endl;
        return 1;
    }

    Converter converter(output);

    char buffer[4096];

    while (true)
    {
        // packet length
        if (fread(buffer, 1, 2, stdin) != 2)
        {
            break;
        }

        UInt16 packetLength = GetUInt16(buffer);

        // read rest of packet
        if (fread(buffer, 1, packetLength, stdin) != packetLength)
        {
            std::cerr << "File malformed" << std::endl;
            fclose(output);
            return 1;
        }

        Itch::Decode(&converter, buffer);
    }

    fclose(output);
    return 0;
}
