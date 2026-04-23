#include "BookEntryList.h"
#include "BookEntry.h"

MLB::BookEntryList::BookEntryList()
    : head(0),
      tail(0)
{
}

MLB::BookEntryList::~BookEntryList()
{
}

void MLB::BookEntryList::pushBack(BookEntry* entry)
{
    entry->prev = tail;
    entry->next = 0;

    if (tail)
    {
        tail->next = entry;
    }
    else
    {
        head = entry;
    }

    tail = entry;
}

void MLB::BookEntryList::remove(BookEntry* entry)
{
    if (entry->prev)
    {
        entry->prev->next = entry->next;
    }
    else
    {
        head = entry->next;
    }

    if (entry->next)
    {
        entry->next->prev = entry->prev;
    }
    else
    {
        tail = entry->prev;
    }

    entry->prev = 0;
    entry->next = 0;
}

MLB::BookEntry* MLB::BookEntryList::front() const
{
    return head;
}

bool MLB::BookEntryList::empty() const
{
    return head == 0;
}
