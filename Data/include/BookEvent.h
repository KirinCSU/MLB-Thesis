#ifndef BOOK_EVENT_H
#define BOOK_EVENT_H

#include "Event.h"
#include "Spec.h"
#include "OrderReferenceNumber.h"

namespace MLB
{

class BookEvent : public Event
{
public:
    enum Action
    {
        Addition,
        Deletion,
        Revision,
        Execution,
        HidTrade,
        InsideQuote,
        InsideTrade
    };

public:
    Action action;
    const Spec* spec;
    OrderReferenceNumber orn;

private:
    bool buy_;
    int size_;
    double price_;
    OrderReferenceNumber match_;

public:
    BookEvent(Publisher* publisher, Time eventTime);
    ~BookEvent();

    // Modifiers
    BookEvent* setAddition(const Spec* spec, OrderReferenceNumber orn, bool buy, int size, double price);
    BookEvent* setDeletion(const Spec* spec, OrderReferenceNumber orn);
    BookEvent* setDeletion(const Spec* spec, OrderReferenceNumber orn, int size);
    BookEvent* setRevision(const Spec* spec, OrderReferenceNumber orn, int size, double price);
    BookEvent* setExecution(const Spec* spec, OrderReferenceNumber orn, int size);
    BookEvent* setExecution(const Spec* spec, OrderReferenceNumber orn, int size, OrderReferenceNumber match);
    BookEvent* setExecution(const Spec* spec, OrderReferenceNumber orn, int size, OrderReferenceNumber match, double price);
    BookEvent* setHidTrade(const Spec* spec, OrderReferenceNumber orn, bool buy, int size, double price);
    BookEvent* setHidTrade(const Spec* spec, OrderReferenceNumber orn, bool buy, int size, double price, OrderReferenceNumber match);
    BookEvent* setInsideQuote(const Spec* spec, bool buy, int size, double price);
    BookEvent* setInsideTrade(const Spec* spec, bool buy, int size, double price, Time insertionTime);

    // Accessors
    bool buy() const;
    int size() const;
    double price() const;
    OrderReferenceNumber match() const;
    Time iTime() const;

    bool isAddition() const;
    bool isDeletion() const;
    bool isRevision() const;
    bool isExecution() const;
    bool isHidTrade() const;
    bool isInsideQuote() const;
    bool isInsideTrade() const;

private:
    BookEvent();
    BookEvent(const BookEvent& rhs);
    BookEvent& operator=(const BookEvent& rhs);
};

}

#endif
