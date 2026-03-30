#ifndef ITCH_BASE
#define ITCH_BASE

#include "Elements.h"
#include "Helpers.h"

namespace MLB
{

    namespace Itch
    {

        struct SystemEvent
        {
            // MsgType 'S'
            char msgType;           // Value "S"
            char stockLocate[2];    // Always 0
            char trackingNumber[2]; // NASDAQ OMX internal tracking number
            char timeStamp[6];      // Nanoseconds since midnight
            char eventCode;         // "O" - start of messages
                                    // "S" - start of system hours
                                    // "Q" - start of market hours
                                    // "M" - end of market hours
                                    // "E" - end of system hours
                                    // "C" - end of messagees
        };

        struct StockDirectory
        {
            // MsgType 'R'
            char msgType;           // Always 'R'
            char stockLocate[2];    // stock locate code unique for day
            char trackingNumber[2]; // Nasdaq OMX internal tracking number
            char timeStamp[6];      // Nanoseconds since midnight
            char symbol[8];         // Stock symbol
            char marketCategory;    // Exchange (see doc)
            char financialStatus;   // "N" - Default (Normal) (see doc)
            char roundLotSize[4];   // Round lots
            char roundLotsOnly;     // "Y" - only round lots "N" - mix allowed
            char issueClass;        // security class
            char issueSubType[2];   // issue sub type
            char authenticity;      // "P" - live/production "T" - test/demo state
            char shortSaleThres;    // "Y" - close out short sale rule "N" - not restrited
            char ipo;               // "Y" - new IPO "N" - not IPO
            char LULD;              // "1" - Tier 1 NSM "2" - Tier 2 NMS
            char ETP;               // "Y" - ETP "N" - not ETP (exchange traded product)
            char ETPLeverage[4];    // Indicts ETP to underlying index levearge (1..N)
            char ETPInverse;        // "Y" - ETP is inverse "N" - ETP is not inverse
        };

        struct TradingAction
        {
            // MsgType 'H'
            char msgType;           // Alway "H"
            char stockLocate[2];    // stock locate code unique for day
            char trackingNumber[2]; // Nasdaq OMX internal tracking number
            char timeStamp[6];      // Nanoseconds since midnight
            char symbol[8];         // Stock symbol
            char tradingState;      // "H" - Halted "P" - Pause "Q" - Quotation "T" - Trading
            char reserved;          // Reserved
            char reason[4];         // Trading action reason
        };

        struct RegSHORestrict
        {
            // MsgType 'Y'
            char msgType;           // Alway "Y"
            char stockLocate[2];    // stock locate code unique for day
            char trackingNumber[2]; // Nasdaq OMX internal tracking number
            char timeStamp[6];      // Nanoseconds since midnight
            char symbol[8];         // stock symobl
            char regSHOAction;      // "0" - no price test "1" - intraday price drop "2" - remains in effect
        };

        struct ParticipantPosition
        {
            // MsgType 'L'
            char msgType;           // Always "L"
            char stockLocate[2];    // stock locate code unique for day
            char trackingNumber[2]; // Nasdaq OMX internal tracking number
            char timeStamp[6];      // Nanoseconds since midnight
            char MPID[4];           // mpid
            char symbol[8];         // stock symobl
            char primaryMM;         // "Y" - primary market maker "N" - non-primary market maker
            char marketMakerMode;   // (see doc)
            char participantState;  // (see doc)
        };

        struct MWCBLevel
        {
            // MsgType 'V'
            char msgType;           // Alway "V"
            char stockLocate[2];    // always 0
            char trackingNumber[2]; // Nasdaq OMX internal tracking number
            char timeStamp[6];      // Nanoseconds since midnight
            char level1[8];
            char level2[8];
            char level3[8];
        };

        struct MWCBStatus
        {
            // MsgType 'W'
            char msgType;           // Alway "W"
            char stockLocate[2];    // always 0
            char trackingNumber[2]; // Nasdaq OMX internal tracking number
            char timeStamp[6];      // Nanoseconds since midnight
            char breachedLevel;
        };

        struct IPOQuoting
        {
            // MsgType 'K'
            char msgType;             // Alway "K"
            char stockLocate[2];      // always 0
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];      // Nanoseconds since midnight
            char symbol[8];           // Stock symbol
            char ipoRelaseTime[4];
            char ipoReleaseQualifier;
            char ipoPrice[4];
        };

        struct AddOrder
        {
            // MsgType 'A'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char orn[8];
            char side;
            char shares[4];
            char symbol[8];
            char price[4];
        };

        struct AddOrderWithMPID
        {
            // MsgType 'F'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char orn[8];
            char side;
            char shares[4];
            char symbol[8];
            char price[4];
            char MPID[4];
        };

        struct OrderExecuted
        {
            // MsgType 'E'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char orn[8];
            char execShares[4];
            char matchNumber[8];
        };

        struct OrderExecutedWithPrice
        {
            // MsgType 'C'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char orn[8];
            char execShares[4];
            char matchNumber[8];
            char printable;
            char price[4];
        };

        struct OrderCancel
        {
            // MsgType 'X'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char orn[8];
            char cxldShares[4];
        };

        struct OrderDelete
        {
            // MsgType 'D'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char orn[8];
        };

        struct OrderReplace
        {
            // MsgType 'U'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char origOrn[8];
            char newOrn[8];
            char newShares[4];
            char newPrice[4];
        };

        struct Trade
        {
            // MsgType 'P'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char orn[8];
            char side;
            char shares[4];
            char symbol[8];
            char price[4];
            char matchNumber[8];
        };

        struct CrossTrade
        {
            // MsgType 'Q'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char shares[8];
            char symbol[8];
            char price[4];
            char matchNumber[8];
            char crossType;
        };

        struct BrokenTrade
        {
            // MsgType 'B'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char matchNumber[8];
        };

        struct NetOrderImbalance
        {
            // MsgType 'I'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char pairedShares[8];
            char imbalancedShares[8];
            char imbalancedDirection;
            char symbol[8];
            char farPrice[4];
            char nearPrice[4];
            char currReference[4];
            char crossType;
            char priceVariationIndicator;
        };

        struct RetailInterest
        {
            // MsgType 'G'
            char msgType;
            char stockLocate[2];      // locate code for security
            char trackingNumber[2];   // Nasdaq OMX internal tracking number
            char timeStamp[6];        // Nanoseconds since midnight
            char symbol[8];
            char interestFlag;
        };

        class StreamCB
        {
            public:
                virtual ~StreamCB() {}

                virtual void onSystemEventMsg(const SystemEvent* msg) {}
                virtual void onStockDirectoryMsg(const StockDirectory* msg) {}
                virtual void onTradingActionMsg(const TradingAction* msg) {}
                virtual void onRegSHORestrictMsg(const RegSHORestrict* msg) {}
                virtual void onParticipantPositionMsg(const ParticipantPosition* msg) {}
                virtual void onMWCBLevelMsg(const MWCBLevel* msg) {}
                virtual void onMWCBStatusMsg(const MWCBStatus* msg) {}
                virtual void onIPOQuotingMsg(const IPOQuoting* msg) {}
                virtual void onAddOrderMsg(const AddOrder* msg) {}
                virtual void onAddOrderWithMPIDMsg(const AddOrderWithMPID* msg) {}
                virtual void onOrderExecutedMsg(const OrderExecuted* msg) {}
                virtual void onOrderExecutedWithPriceMsg(const OrderExecutedWithPrice* msg) {}
                virtual void onOrderCancelMsg(const OrderCancel* msg) {}
                virtual void onOrderDeleteMsg(const OrderDelete* msg) {}
                virtual void onOrderReplaceMsg(const OrderReplace* msg) {}
                virtual void onTradeMsg(const Trade* msg) {}
                virtual void onCrossTradeMsg(const CrossTrade* msg) {}
                virtual void onBrokenTradeMsg(const BrokenTrade* msg) {}
                virtual void onNetOrderImbalanceMsg(const NetOrderImbalance* msg) {}
                virtual void onRetailInterestMsg(const RetailInterest* msg) {}
        };

        inline void Decode(StreamCB* streamCB, char* msg)
        {
            switch (msg[0])
            {
                case 'S' :
                    streamCB->onSystemEventMsg((const SystemEvent*)msg);
                    std::cout << "S" << std::endl;
                    break;
                case 'R' :
                    streamCB->onStockDirectoryMsg((const StockDirectory*)msg);
                    std::cout << "R" << std::endl;
                    break;
                case 'H' :
                    streamCB->onTradingActionMsg((const TradingAction*)msg);
                    std::cout << "H" << std::endl;
                    break;
                case 'Y' :
                    streamCB->onRegSHORestrictMsg((const RegSHORestrict*)msg);
                    std::cout << "Y" << std::endl;
                    break;
                case 'V' :
                    streamCB->onMWCBLevelMsg((const MWCBLevel*)msg);
                    std::cout << "V" << std::endl;
                    break;
                case 'W' :
                    streamCB->onMWCBStatusMsg((const MWCBStatus*)msg);
                    std::cout << "W" << std::endl;
                    break;
                case 'K' :
                    streamCB->onIPOQuotingMsg((const IPOQuoting*)msg);
                    std::cout << "K" << std::endl;
                    break;
                case 'L' :
                    streamCB->onParticipantPositionMsg((const ParticipantPosition*)msg);
                    std::cout << "L" << std::endl;
                    break;
                case 'A' :
                    streamCB->onAddOrderMsg((const AddOrder*)msg);
                    std::cout << "A" << std::endl;
                    break;
                case 'F' :
                    streamCB->onAddOrderWithMPIDMsg((const AddOrderWithMPID*)msg);
                    std::cout << "F" << std::endl;
                    break;
                case 'E' :
                    streamCB->onOrderExecutedMsg((const OrderExecuted*)msg);
                    std::cout << "E" << std::endl;
                    break;
                case 'C' :
                    streamCB->onOrderExecutedWithPriceMsg((const OrderExecutedWithPrice*)msg);
                    std::cout << "C" << std::endl;
                    break;
                case 'X' :
                    streamCB->onOrderCancelMsg((const OrderCancel*)msg);
                    std::cout << "X" << std::endl;
                    break;
                case 'D' :
                    streamCB->onOrderDeleteMsg((const OrderDelete*)msg);
                    std::cout << "D" << std::endl;
                    break;
                case 'U' :
                    streamCB->onOrderReplaceMsg((const OrderReplace*)msg);
                    std::cout << "U" << std::endl;
                    break;
                case 'P' :
                    streamCB->onTradeMsg((const Trade*)msg);
                    std::cout << "P" << std::endl;
                    break;
                case 'Q' :
                    streamCB->onCrossTradeMsg((const CrossTrade*)msg);
                    std::cout << "Q" << std::endl;
                    break;
                case 'B' :
                    streamCB->onBrokenTradeMsg((const BrokenTrade*)msg);
                    std::cout << "B" << std::endl;
                    break;
                case 'I' :
                    streamCB->onNetOrderImbalanceMsg((const NetOrderImbalance*)msg);
                    std::cout << "I" << std::endl;
                    break;
                case 'G' :
                    streamCB->onRetailInterestMsg((const RetailInterest*)msg);
                    std::cout << "G" << std::endl;
                    break;
                default :
                    std::cout << "Default" << std::endl;
                    break;
            }
        }

        inline uint64_t getNsecFromMidnight(const char* timeStamp)
        {
            UInt64 result = 0;
            result =   (((UInt64)timeStamp[0] & 0xFF) << 40) |
                (((UInt64)timeStamp[1] & 0xFF) << 32) |
                (((UInt64)timeStamp[2] & 0xFF) << 24) |
                (((UInt64)timeStamp[3] & 0xFF) << 16) |
                (((UInt64)timeStamp[4] & 0xFF) << 8)  |
                (((UInt64)timeStamp[5] & 0xFF) << 0);

            return result;
        }

        inline int Length(const char* symbol)
        {
            int i = 7;
            while (i >= 0)
            {
                if (symbol[i] != ' ')
                {
                    break;
                }
                i--;
            }
            return i + 1;

        }
    }
}

#endif
