#ifndef BOOK_ENTRY_LIST_H
#define BOOK_ENTRY_LIST_H

#include "BookEntry.h"

namespace MLB
{

class BookEntryList
{
private:
    BookEntry* head;
    BookEntry* tail;

public:
    BookEntryList();
    ~BookEntryList();

    void pushBack(BookEntry* entry);
    void remove(BookEntry* entry);

    BookEntry* front() const;
    bool empty() const;
};

}

#endif
