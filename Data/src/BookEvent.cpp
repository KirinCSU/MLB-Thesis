#include "BookEvent.h"

MLB::BookEvent::BookEvent(Publisher* a_publisher, Time a_eventTime)
    : Event(a_publisher, a_eventTime, Event::Book),
      spec(0),
      orn(0)
{
    buy_ = true;
    size_ = 0;
    price_ = 0.0;
    match_ = 0;
}

MLB::BookEvent::~BookEvent()
{
}

MLB::BookEvent* MLB::BookEvent::setAddition(const Spec* a_spec, OrderReferenceNumber a_orn, bool a_buy, int a_size, double a_price)
{
    action = Addition;
    spec = a_spec;
    orn = a_orn;

    buy_ = a_buy;
    size_ = a_size;
    price_ = a_price;
    return this;
}

MLB::BookEvent* MLB::BookEvent::setDeletion(const Spec* a_spec, OrderReferenceNumber a_orn)
{
    return setDeletion(a_spec, a_orn, 0);
}

MLB::BookEvent* MLB::BookEvent::setDeletion(const Spec* a_spec, OrderReferenceNumber a_orn, int a_size)
{
    action = Deletion;
    spec = a_spec;
    orn = a_orn;

    size_ = a_size;
    return this;
}

MLB::BookEvent* MLB::BookEvent::setRevision(const Spec* a_spec, OrderReferenceNumber a_orn, int a_size, double a_price)
{
    action = Revision;
    spec = a_spec;
    orn = a_orn;

    size_ = a_size;
    price_ = a_price;
    return this;
}

MLB::BookEvent* MLB::BookEvent::setExecution(const Spec* a_spec, OrderReferenceNumber a_orn, int a_size)
{
    action = Execution;
    spec = a_spec;
    orn = a_orn;

    size_ = a_size;
    return this;
}

MLB::BookEvent* MLB::BookEvent::setExecution(const Spec* a_spec, OrderReferenceNumber a_orn, int a_size, OrderReferenceNumber a_match)
{
    match_ = a_match;
    price_ = 0.0;
    return setExecution(a_spec, a_orn, a_size);
}

MLB::BookEvent* MLB::BookEvent::setExecution(const Spec* a_spec, OrderReferenceNumber a_orn, int a_size, OrderReferenceNumber a_match, double a_price)
{
    match_ = a_match;
    price_ = a_price;
    return setExecution(a_spec, a_orn, a_size);
}

MLB::BookEvent* MLB::BookEvent::setHidTrade(const Spec* a_spec, OrderReferenceNumber a_orn, bool a_buy, int a_size, double a_price)
{
    action = HidTrade;
    spec = a_spec;
    orn = a_orn;

    buy_ = a_buy;
    size_ = a_size;
    price_ = a_price;
    return this;
}

MLB::BookEvent* MLB::BookEvent::setHidTrade(const Spec* a_spec, OrderReferenceNumber a_orn, bool a_buy, int a_size, double a_price, OrderReferenceNumber a_match)
{
    match_ = a_match;
    return setHidTrade(a_spec, a_orn, a_buy, a_size, a_price);
}

MLB::BookEvent* MLB::BookEvent::setInsideQuote(const Spec* a_spec, bool a_buy, int a_size, double a_price)
{
    action = InsideQuote;
    spec = a_spec;
    orn = 0;

    buy_ = a_buy;
    size_ = a_size;
    price_ = a_price;
    return this;
}

MLB::BookEvent* MLB::BookEvent::setInsideTrade(const Spec* a_spec, bool a_buy, int a_size, double a_price, Time insertionTime)
{
    action = InsideTrade;
    spec = a_spec;
    orn = insertionTime.usec + 1000000ULL * insertionTime.sec;

    buy_ = a_buy;
    size_ = a_size;
    price_ = a_price;
    return this;
}

bool MLB::BookEvent::buy() const
{
    return buy_;
}

int MLB::BookEvent::size() const
{
    return size_;
}

double MLB::BookEvent::price() const
{
    return price_;
}

MLB::OrderReferenceNumber MLB::BookEvent::match() const
{
    return match_;
}

MLB::Time MLB::BookEvent::iTime() const
{
    return Time(orn);
}

bool MLB::BookEvent::isAddition() const
{
    return (action == Addition);
}

bool MLB::BookEvent::isDeletion() const
{
    return (action == Deletion);
}

bool MLB::BookEvent::isRevision() const
{
    return (action == Revision);
}

bool MLB::BookEvent::isExecution() const
{
    return (action == Execution);
}

bool MLB::BookEvent::isHidTrade() const
{
    return (action == HidTrade);
}

bool MLB::BookEvent::isInsideQuote() const
{
    return (action == InsideQuote);
}

bool MLB::BookEvent::isInsideTrade() const
{
    return (action == InsideTrade);
}
