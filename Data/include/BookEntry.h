#ifndef BOOK_ENTRY_H
#define BOOK_ENTRY_H

#include "OrderReferenceNumber.h"

namespace MLB
{

class BookEntry
{
public:
    OrderReferenceNumber orn;
    bool buy;
    int size;
    double price;

    BookEntry* prev;
    BookEntry* next;

public:
    BookEntry(OrderReferenceNumber orn,
              bool buy,
              int size,
              double price);

    ~BookEntry();
};

}

#endif
