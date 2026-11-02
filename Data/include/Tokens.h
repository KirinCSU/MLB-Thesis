#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>
#include "Loaders.h"

namespace MLB
{

    //Book State Tokens
    enum class Spread : char
    {
        S = 'S', //Small
        M = 'M', //Medium
        L = 'L'  //Large
    };

    //Book Event Tokens
    enum class EventType : char
    {
        A = 'A', //Addition
        D = 'D', //Deletion
        R = 'R', //Revision
        E = 'E', //Execute
        H = 'H', //Hidden
        Q = 'Q', //Quote
        I = 'I', //Inside Trade
    };

    enum class EventDepth : char
    {
        T = 'T', //Top of Book
        I = 'I', //Inside of Book
    };

    enum class EventSide : char
    {
        B = 'B', //Buy Side
        S = 'S'  //Sell Side
    };

    enum class MidMove : char
    {
        B = 'B', //Moved toward Bid
        F = 'F', //Didn't Move
        A = 'A'  //Moved toward Ask
    };

    enum class MidMoveSize : char
    {
        S = 'S', //Small
        M = 'M', //Medium
        L = 'L'  //Large
    };

    struct Token
    {
        //Pre Field
        double preMid;

        //Book State
        double spread;

        double topImbalance;
        double innerImbalance;

        //Event
        EventType eventType;
        double eventLevelSize;
        double eventTotalSize;
        EventDepth eventDepth;
        EventSide eventSide;

        MidMove midMove;
        double  midMoveSize;
    };

    void PrintToken(const Token& token)
    {
        std::cout << std::fixed << std::setprecision(3) << "[" << token.spread << " - " << token.topImbalance << "," << token.innerImbalance << " - " << static_cast<char>(token.eventType) << "," << token.eventLevelSize << ", " << token.eventTotalSize << ", " << static_cast<char>(token.eventDepth) << "," << static_cast<char>(token.eventSide) << " - " << static_cast<char>(token.midMove) << "," << token.midMoveSize << "]" << std::endl;
    }

    void FillState(Token* token, BookEvent* bookEvent, Books* books, const Vols& vols)
    {
        //Pre Field
        Book* book = books->get(bookEvent->spec);
        token->preMid = book->mid();

        //Book State
        double scaled = token->preMid * vols.at(bookEvent->spec->ticker);
        token->spread = book->spread() / scaled;

        double boundary = book->spread();
        double bestBidVolume = book->buyVolumeAt(book->bestBid());
        double bestAskVolume = book->sellVolumeAt(book->bestAsk());

        if(bestBidVolume + bestAskVolume > 0.0)
        {
            token->topImbalance = (bestBidVolume - bestAskVolume) / (bestBidVolume + bestAskVolume);
        }
        else
        {
            token->topImbalance = 0.0;
        }

        double innerBidVolume = book->buyVolumeIn(book->bestBid() - boundary, book->bestBid()) - bestBidVolume;
        double innerAskVolume = book->sellVolumeIn(book->bestAsk(), book->bestAsk() + boundary) - bestAskVolume; 

        if(innerBidVolume + innerAskVolume > 0.0)
        {
            token->innerImbalance = (innerBidVolume - innerAskVolume) / (innerBidVolume + innerAskVolume);
        }
        else
        {
            token->innerImbalance = 0.0;
        }

        //Event
        double eventPrice = bookEvent->price();
        double eventSize = bookEvent->size();
        double doubleCount = false;
        bool buy = bookEvent->buy();

        switch (bookEvent->action)
        {
            case BookEvent::Addition:
                token->eventType = EventType::A;
                break;
            case BookEvent::Deletion:
                token->eventType = EventType::D;
                eventPrice = book->price(bookEvent->orn);
                eventSize = book->size(bookEvent->orn);
                buy = book->side(bookEvent->orn);
                doubleCount = true;
                break;
            case BookEvent::Revision:
                token->eventType = EventType::R;
                break;
            case BookEvent::Execution:
                token->eventType = EventType::E;
                eventPrice = book->price(bookEvent->orn);
                buy = book->side(bookEvent->orn);
                doubleCount = true;
                break;
            case BookEvent::HidTrade:
                token->eventType = EventType::H;
                break;
            case BookEvent::InsideQuote:
                token->eventType = EventType::Q;
                break;
            case BookEvent::InsideTrade:
                token->eventType = EventType::I;
                break;
        }

        //std::cout << book->bestBid() << " " << book->bestAsk() << " " << eventPrice << " " << eventSize << static_cast<char>(token->eventType) << std::endl;

        token->eventDepth = EventDepth::I;

        token->eventLevelSize = 0.0;
        token->eventTotalSize = 0.0;

        if(buy)
        {
            double levelVolume = book->buyVolumeAt(eventPrice) + eventSize;
            double totalVolume = book->buyVolumeIn(book->bestBid() - boundary, book->bestBid()) + eventSize;

            if(doubleCount)
            {
                levelVolume -= eventSize;
                totalVolume -= eventSize;
            }

            if(eventSize != 0.0)
            {
                token->eventLevelSize = eventSize / levelVolume;
                token->eventTotalSize = eventSize / totalVolume;
            }

            if(eventPrice >= book->bestBid())
            {
                token->eventDepth = EventDepth::T;
            }

            token->eventSide = EventSide::B;
        }
        else
        {
            double levelVolume = book->sellVolumeAt(eventPrice) + eventSize;
            double totalVolume = book->sellVolumeIn(book->bestAsk(), book->bestAsk() + boundary) + eventSize;

            if(doubleCount)
            {
                levelVolume -= eventSize;
                totalVolume -= eventSize;
            }

            if(eventSize != 0.0)
            {
                token->eventLevelSize = eventSize / levelVolume;
                token->eventTotalSize = eventSize / totalVolume;
            }

            if(eventPrice <= book->bestAsk())
            {
                token->eventDepth = EventDepth::T;
            }

            token->eventSide = EventSide::S;
        }
    }

    void FillChange(Token* token, Book* book, const Vols& vols)
    {
        double newMid = book->mid();

        token->midMove = MidMove::F;

        if(newMid > token->preMid)
        {
            token->midMove = MidMove::A;
        }
        else if(newMid < token->preMid)
        {
            token->midMove = MidMove::B;
        }

        double scaled = token->preMid * vols.at(book->spec->ticker);

        if(scaled != 0.0)
        {
            token->midMoveSize = (newMid - token->preMid) / scaled;
        }
        else
        {
            token->midMoveSize = 0.0;
        }

    }
}

#endif
