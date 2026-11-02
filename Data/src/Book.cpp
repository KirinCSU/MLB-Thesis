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

    //ADDING VOL
    BookEntryList& level = entry->buy ? bids[entry->price] : asks[entry->price];
    level.volume -= event->size();

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

const std::map<double, MLB::BookEntryList>& MLB::Book::getBids() const
{
    return bids;
}

const std::map<double, MLB::BookEntryList>& MLB::Book::getAsks() const
{
    return asks;
}

double MLB::Book::bestBid() const
{
    double bid = 0.0;

    if(!bids.empty())
    {
        for(auto it = bids.rbegin(); it != bids.rend(); ++it)
        {
            if(it->second.volume > 0.0)
            {
                bid = it->first;
                break;
            }
        }
    }

    return bid;
} 

double MLB::Book::bestAsk() const
{
    double ask = 0.0;

    if(!asks.empty())
    {
        for(auto it = asks.begin(); it != asks.end(); ++it)
        {
            if(it->second.volume > 0.0)
            {
                ask = it->first;
                break;
            }
        }
    }

    return ask;
}

double MLB::Book::mid() const
{
    double bid = bestBid();
    double ask = bestAsk();

    if(bid == 0.0 || ask == 0.0)
    {
        return 0.0;
    }

    return (bid + ask) / 2.0;
}

double MLB::Book::spread() const
{
    double bid = bestBid();
    double ask = bestAsk();

    if(bid == 0.0 || ask == 0.0)
    {
        return 0.0;
    }

    return ask - bid;
}

double MLB::Book::totalVolume() const
{
    double volume = 0.0;

    if(!bids.empty())
    {
        for(auto it = bids.rbegin(); it != bids.rend(); ++it)
        {
            volume += it->second.volume;
        }
    }

    if(!asks.empty())
    {
        for(auto it = asks.rbegin(); it != asks.rend(); ++it)
        {
            volume += it->second.volume;
        }
    }

    return volume;
}

double MLB::Book::buyVolumeAt(double price) const
{
    auto it = bids.find(price);

    if(it != bids.end())
    {
        return it->second.volume;
    }

    return 0.0;
}

double MLB::Book::sellVolumeAt(double price) const
{
    auto it = asks.find(price);

    if(it != asks.end())
    {
        return it->second.volume;
    }

    return 0.0;
}

double MLB::Book::buyVolumeIn(double low, double high) const
{
    if(low > high)
    {
        return 0.0;
    }

    double volume = 0.0;

    for(auto it = bids.lower_bound(low); it != bids.upper_bound(high); ++it)
    {
        volume += it->second.volume;
    }

    return volume;
}

double MLB::Book::sellVolumeIn(double low, double high) const
{
    if(low > high)
    {
        return 0.0;
    }

    double volume = 0.0;

    for(auto it = asks.lower_bound(low); it != asks.upper_bound(high); ++it)
    {
        volume += it->second.volume;
    }

    return volume;
}

double MLB::Book::price(OrderReferenceNumber orn) const
{
    auto it = entries.find(orn);
    if (it == entries.end())
    {
        return 0.0;
    }

    BookEntry* entry = it->second;

    return entry->price;
}

int MLB::Book::size(OrderReferenceNumber orn) const
{
    auto it = entries.find(orn);
    if (it == entries.end())
    {
        return 0;
    }

    BookEntry* entry = it->second;

    return entry->size;
}

bool MLB::Book::side(OrderReferenceNumber orn) const
{
    auto it = entries.find(orn);
    if (it == entries.end())
    {
        return 0;
    }

    BookEntry* entry = it->second;

    return entry->buy;
}
