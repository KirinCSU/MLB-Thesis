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
    const Spec* spec;

    std::map<OrderReferenceNumber, BookEntry*> entries;

    std::map<double, BookEntryList> bids;
    std::map<double, BookEntryList> asks;

public:
    Book(const Spec* spec);
    ~Book();

    void onEvent(const BookEvent* event);

    bool empty() const;
    int size() const;

private:
    void handleAddition(const BookEvent* event);
    void handleDeletion(const BookEvent* event);
    void handleExecution(const BookEvent* event);
    void handleRevision(const BookEvent* event);
};

}

#endif
