#include <cstdint>
#include <cstdio>
#include <iostream>
#include <set>
#include <map>
#include <vector>

#include "ItchBase.h"
#include "Elements.h"
#include "Helpers.h"
#include "OrderReferenceNumber.h"
#include "Time.h"

using namespace MLB;

class CallBackTest : public Itch::StreamCB
{

    private:
        typedef std::map<OrderReferenceNumber, OrderReferenceNumber> Revisions;
        typedef std::map<OrderReferenceNumber, PairII> ORNToInfo;

    private:
        Ints prices;
        MapSI tickerIDs;

        ORNToInfo ornToInfo;
        Revisions revisions;

    public:

        void onSystemEventMsg(const Itch::SystemEvent* msg)
        {

            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "SystemEventMsg" << std::endl;

            std::cout << "MsgType         " << msg->msgType << std::endl;
            std::cout << "StockLocate     " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp       " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "Event Code      " << msg->eventCode << std::endl;

            std::cout << std::endl;
        }

        void onStockDirectoryMsg(const Itch::StockDirectory* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "StockDirectoryMsg" << std::endl;

            std::cout << "MsgType          " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate      " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number  " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp        " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "Symbol           " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "Market Category  " << msg->marketCategory << std::endl;    // Exchange (see doc)
            std::cout << "Financial Status " << msg->financialStatus << std::endl;   // "N" - Default (Normal) (see doc)
            std::cout << "Round Lot Size   " << GetUInt32(msg->roundLotSize) << std::endl;   // Round lots
            std::cout << "Round Lots Only  " << msg->roundLotsOnly << std::endl;     // "Y" - only round lots "N" - mix allowed
            std::cout << "Issue Class      " << msg->issueClass << std::endl;        // security class
            std::cout << "Issue Sub Type   " << msg->issueSubType[0] << msg->issueSubType[1] << std::endl;   // issue sub type
            std::cout << "Authenticity     " << msg->authenticity << std::endl;      // "P" - live/production "T" - test/demo state
            std::cout << "Short Sale Thres " << msg->shortSaleThres << std::endl;    // "Y" - close out short sale rule "N" - not restrited
            std::cout << "IPO              " << msg->ipo << std::endl;               // "Y" - new IPO "N" - not IPO
            std::cout << "LULD             " << msg->LULD << std::endl;              // "1" - Tier 1 NSM "2" - Tier 2 NMS
            std::cout << "ETP              " << msg->ETP << std::endl;               // "Y" - ETP "N" - not ETP (exchange traded product)
            std::cout << "ETP Leverage     " << GetUInt32(msg->ETPLeverage) << std::endl;       // Indicts ETP to underlying index levearge (1..N)
            std::cout << "ETP Inverse      " << msg->ETPInverse << std::endl;        // "Y" - ETP is inverse "N" - ETP is not inverse

            std::cout << std::endl;
        }

        void onTradingActionMsg(const Itch::TradingAction* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "TradingActionMsg" << std::endl;

            std::cout << "MsgType          " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate      " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number  " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp        " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "Symbol           " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "TradingState     " << msg->tradingState << std::endl;   // "H" - Halted "P" - Pause "Q" - Quotation "T" - Trading
            std::cout << "Reserved         " << msg->reserved << std::endl;       // Reserved
            std::cout << "Reason           " << msg->reason[0] << msg->reason[1] << msg->reason[2] << msg->reason[3] << std::endl;         // Trading action reason

            std::cout << std::endl;
        }

        void onRegSHORestrictMsg(const Itch::RegSHORestrict* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "RegSHORestrict" << std::endl;

            std::cout << "MsgType          " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate      " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number  " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp        " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "Symbol           " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "RegSHOAction     " << msg->regSHOAction << std::endl;      // "0" - no price test "1" - intraday price drop "2" - remains in effect
            std::cout << std::endl;

            std::cout << std::endl;
        }

        void onParticipantPositionMsg(const Itch::ParticipantPosition* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "ParticipantPositionMsg" << std::endl;

            std::cout << "MsgType          " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate      " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number  " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp        " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "MPID             " << msg->MPID[0] << msg->MPID[1] << msg->MPID[2] << msg->MPID[3] << std::endl;           // mpid
            std::cout << "Symbol           " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "PrimaryMM        " << msg->primaryMM << std::endl;         // "Y" - primary market maker "N" - non-primary market maker
            std::cout << "MarketMakerMode  " << msg->marketMakerMode << std::endl;   // (see doc)
            std::cout << "ParticipantState " << msg->participantState << std::endl;  // (see doc)

            std::cout << std::endl;
        }

        void onMWCBLevelMsg(const Itch::MWCBLevel* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "MWCBLevel" << std::endl;

            std::cout << "MsgType          " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate      " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number  " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp        " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "Leve1            " << GetUInt64(msg->level1) << std::endl;
            std::cout << "Leve2            " << GetUInt64(msg->level2) << std::endl;
            std::cout << "Leve3            " << GetUInt64(msg->level3) << std::endl;

            std::cout << std::endl;
        }

        void onMWCBStatusMsg(const Itch::MWCBStatus* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "MWCBStatus" << std::endl;

            std::cout << "MsgType          " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate      " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number  " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp        " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "BreachedLevel    " << msg->breachedLevel << std::endl;

            std::cout << std::endl;
        }

        void onIPOQuotingMsg(const Itch::IPOQuoting* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "MWCBStatus" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "Symbol             " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "IpoReleaseTime     " << GetUInt32(msg->ipoRelaseTime) << std::endl;
            std::cout << "IpoReleaseQualifer " << msg->ipoReleaseQualifier << std::endl;
            std::cout << "IpoPrice           " << GetUInt32(msg->ipoPrice) << std::endl;

        }

        void onAddOrderMsg(const Itch::AddOrder* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "AddOrderMsg" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "OrderRefNumber     " << GetUInt64(msg->orn) << std::endl;
            std::cout << "Side               " << msg->side << std::endl;
            std::cout << "Shares             " << GetUInt32(msg->shares) << std::endl;;
            std::cout << "Symbol             " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "Price              " <<  GetUInt32(msg->price) << std::endl;

            std::cout << std::endl;
        }

        void onAddOrderWithMPIDMsg(const Itch::AddOrderWithMPID* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "AddOrderMsgWithMPID" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "OrderRefNumber     " << GetUInt64(msg->orn) << std::endl;
            std::cout << "Side               " << msg->side << std::endl;
            std::cout << "Shares             " << GetUInt32(msg->shares) << std::endl;;
            std::cout << "Symbol             " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "Price              " <<  GetUInt32(msg->price) << std::endl;
            std::cout << "MPID               " << msg->MPID[0] << msg->MPID[1] << msg->MPID[2] << msg->MPID[3] << std::endl;           // mpid

            std::cout << std::endl;
        }

        void onOrderExecutedMsg(const Itch::OrderExecuted* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "OrderExecutedMsg" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "OrderRefNumber     " << GetUInt64(msg->orn) << std::endl;
            std::cout << "Shares             " << GetUInt32(msg->execShares) << std::endl;
            std::cout << "MatchNumber        " << GetUInt64(msg->matchNumber) << std::endl;;

            std::cout << std::endl;
        }

        void onOrderExecutedWithPriceMsg(const Itch::OrderExecutedWithPrice* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "OrderExecutedMsgWithPrice" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "OrderRefNumber     " << GetUInt64(msg->orn) << std::endl;
            std::cout << "Shares             " << GetUInt32(msg->execShares) << std::endl;
            std::cout << "MatchNumber        " << GetUInt64(msg->matchNumber) << std::endl;;
            std::cout << "Printable          " << msg->printable << std::endl;
            std::cout << "Price              " << GetUInt32(msg->price) << std::endl;

            std::cout << std::endl;
        }

        void onOrderCancelMsg(const Itch::OrderCancel* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "OrderCancelMsg" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "OrderRefNumber     " << GetUInt64(msg->orn) << std::endl;
            std::cout << "CanceledShares     " << GetUInt32(msg->cxldShares) << std::endl;

            std::cout << std::endl;
        }

        void onOrderDeleteMsg(const Itch::OrderDelete* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "OrderDeleteMsg" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "OrderRefNumber     " << GetUInt64(msg->orn) << std::endl;

            std::cout << std::endl;
        }

        void onOrderReplaceMsg(const Itch::OrderReplace* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "OrderReplaceMsg" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "OrignalOrn         " << GetUInt64(msg->origOrn) << std::endl;
            std::cout << "NewOrn             " << GetUInt64(msg->newOrn) << std::endl;;
            std::cout << "NewShares          " << GetUInt32(msg->newShares) << std::endl;;
            std::cout << "NewPrice           " << GetUInt32(msg->newPrice) << std::endl;;

            std::cout << std::endl;
        }

        void onTradeMsg(const Itch::Trade* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "TradeMsg" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "OrderRefNumber     " << GetUInt64(msg->orn) << std::endl;
            std::cout << "Side               " << msg->side << std::endl;
            std::cout << "Shares             " << GetUInt32(msg->shares) << std::endl;
            std::cout << "Symbol             " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "Price              " << GetUInt32(msg->price) << std::endl;
            std::cout << "MatchNumber        " << GetUInt64(msg->matchNumber) << std::endl;;

            std::cout << std::endl;
        }

        void onCrossTradeMsg(const Itch::CrossTrade* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "CrossTradeMsg" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;  
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "Shares             " << GetUInt64(msg->shares) << std::endl;
            std::cout << "Symbol             " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "Price              " << GetUInt32(msg->price) << std::endl;
            std::cout << "Match Number       " << GetUInt64(msg->matchNumber) << std::endl;
            std::cout << "CrossType          " << msg->crossType << std::endl;

            std::cout << std::endl;

        }

        void onBrokenTradeMsg(const Itch::BrokenTrade* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "BrokenTradeMsg" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "MatchNumber        " << GetUInt64(msg->matchNumber) << std::endl;;

            std::cout << std::endl;
        }

        void onNetOrderImbalanceMsg(const Itch::NetOrderImbalance* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "NetOrderImbalance" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " << Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "PairedShares       " << GetUInt64(msg->pairedShares) << std::endl;
            std::cout << "ImbalancedShares   " << GetUInt64(msg->imbalancedShares) << std::endl;
            std::cout << "ImbalanceDirection " << msg->imbalancedDirection << std::endl;
            std::cout << "Symbol             " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "FarPrice           " << GetUInt32(msg->farPrice) << std::endl;
            std::cout << "NearPrice          " << GetUInt32(msg->nearPrice) << std::endl;
            std::cout << "CurrentRefPrice    " << GetUInt32(msg->currReference) << std::endl;
            std::cout << "CrossType          " << msg->crossType << std::endl;
            std::cout << "PriceVarIndicator  " << msg->priceVariationIndicator << std::endl;

            std::cout << std::endl;
        }

        void onRetailInterestMsg(const Itch::RetailInterest* msg)
        {
            std::cout << "***---NewEvent---***" << std::endl;
            std::cout << "RetailInterestMsg" << std::endl;

            std::cout << "MsgType            " << msg->msgType << std::endl;           // Always 'R'
            std::cout << "StockLocate        " << GetUInt16(msg->stockLocate) << std::endl;
            std::cout << "Tracking Number    " << GetUInt16(msg->trackingNumber) << std::endl;
            std::cout << "TimeStamp          " <<  Time(Itch::getNsecFromMidnight(msg->timeStamp)/1000).str() << std::endl;      // Nanoseconds since midnight
            std::cout << "Symbol             " << String(msg->symbol,Itch::Length(msg->symbol)) << std::endl;         // Stock symbol
            std::cout << "InterestFlag       " << msg->interestFlag << std::endl;
            std::cout << std::endl;
        }


        int getTickerID(const std::string& ticker)
        {
            MapSI::iterator find = tickerIDs.find(ticker);
            if (find != tickerIDs.end())
            {
                return find->second;
            }

            // New Ticker
            int tickerID = tickerIDs.size();
            tickerIDs[ticker] = tickerID;
            prices.push_back(0);

            return tickerID;
        }

        int getOffset(int tickerID, int price)
        {
            int diff = price - prices[tickerID];
            if (diff % 100 != 0 || diff > 700 || diff < -800)
            {
                // Need New Price
                prices[tickerID] = price;
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

};

int main()
{
    CallBackTest streamCBTest;

    char Buffer[4096];

    int Iterations = 10;

    for(int i = 0; i < Iterations; i++)
    {
        if(fread(Buffer, 1, 2, stdin) != 2)
        {
            std::cout << "Failed to read size bytes" << std::endl;
            return 1;
        }

        UInt16 Size = GetUInt16(Buffer);

        if(fread(Buffer, 1, Size, stdin) != Size)
        {
            std::cout << "Failed to read following bytes!" << std::endl;
            return 1;
        }

        std::cout << "Correctly read message!" << std::endl;

        Itch::Decode(&streamCBTest, Buffer);
    }

    return 0;
}
