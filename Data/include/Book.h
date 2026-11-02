#ifndef BOOK_H
#define BOOK_H

#include "BookEntry.h"
#include "BookEntryList.h"
#include "BookEvent.h"
#include "Spec.h"

#include <map>

namespace MLB
{

class Book
{
private:
    std::map<OrderReferenceNumber, BookEntry*> entries;

    std::map<double, BookEntryList> bids;
    std::map<double, BookEntryList> asks;

public:
    const Spec* spec;

    Book(const Spec* spec);
    ~Book();

    void onEvent(const BookEvent* event);

    bool empty() const;
    int size() const;

    const std::map<double, BookEntryList>& getBids() const;
    const std::map<double, BookEntryList>& getAsks() const;

    double mid() const;
    double spread() const;
    double bestBid() const;
    double bestAsk() const;

    double price(OrderReferenceNumber orn) const;
    int size(OrderReferenceNumber orn) const;
    bool side(OrderReferenceNumber orn) const;

    double totalVolume() const;
    double buyVolumeAt(double price) const;
    double sellVolumeAt(double price) const;
    double buyVolumeIn(double low, double high) const;
    double sellVolumeIn(double low, double high) const;

private:
    void handleAddition(const BookEvent* event);
    void handleDeletion(const BookEvent* event);
    void handleExecution(const BookEvent* event);
    void handleRevision(const BookEvent* event);
};

}

#endif
