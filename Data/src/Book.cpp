#include "Book.h"

MLB::Book::Book(const Spec* a_spec)
    : spec(a_spec)
{
}

MLB::Book::~Book()
{
    for (auto& kv : entries)
    {
        delete kv.second;
    }
}

void MLB::Book::onEvent(const BookEvent* event)
{
    if (event->isAddition())
    {
        handleAddition(event);
    }
    else if (event->isDeletion())
    {
        handleDeletion(event);
    }
    else if (event->isExecution())
    {
        handleExecution(event);
    }
    else if (event->isRevision())
    {
        handleRevision(event);
    }
    else if (event->isHidTrade())
    {
        // no-op for book
    }
}

bool MLB::Book::empty() const
{
    return entries.empty();
}

int MLB::Book::size() const
{
    return entries.size();
}

void MLB::Book::handleAddition(const BookEvent* event)
{
    BookEntry* entry = new BookEntry(
        event->orn,
        event->buy(),
        event->size(),
        event->price()
    );

    entries[event->orn] = entry;

    if (entry->buy)
    {
        bids[entry->price].pushBack(entry);
    }
    else
    {
        asks[entry->price].pushBack(entry);
    }
}

void MLB::Book::handleDeletion(const BookEvent* event)
{
    auto it = entries.find(event->orn);
    if (it == entries.end())
    {
        return;
    }

    BookEntry* entry = it->second;

    if (entry->buy)
    {
        bids[entry->price].remove(entry);
    }
    else
    {
        asks[entry->price].remove(entry);
    }

    delete entry;
    entries.erase(it);
}

void MLB::Book::handleExecution(const BookEvent* event)
{
    auto it = entries.find(event->orn);
    if (it == entries.end())
    {
        return;
    }

    BookEntry* entry = it->second;

    entry->size -= event->size();

    if (entry->size <= 0)
    {
        if (entry->buy)
        {
            bids[entry->price].remove(entry);
        }
        else
        {
            asks[entry->price].remove(entry);
        }

        delete entry;
        entries.erase(it);
    }
}

void MLB::Book::handleRevision(const BookEvent* event)
{
    auto it = entries.find(event->orn);
    if (it == entries.end())
    {
        return;
    }

    BookEntry* entry = it->second;

    // remove from old level
    if (entry->buy)
    {
        bids[entry->price].remove(entry);
    }
    else
    {
        asks[entry->price].remove(entry);
    }

    // update values
    entry->size = event->size();
    entry->price = event->price();

    // reinsert
    if (entry->buy)
    {
        bids[entry->price].pushBack(entry);
    }
    else
    {
        asks[entry->price].pushBack(entry);
    }
}
