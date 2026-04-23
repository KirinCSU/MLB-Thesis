#include "Books.h"

MLB::Books::Books()
{
}

MLB::Books::~Books()
{
    std::map<const Spec*, Book*>::iterator it = books.begin();
    while (it != books.end())
    {
        delete it->second;
        ++it;
    }
}

void MLB::Books::onEvent(const BookEvent* event)
{
    if (!event->spec)
    {
        return;
    }

    std::map<const Spec*, Book*>::iterator it = books.find(event->spec);

    if (it == books.end())
    {
        Book* book = new Book(event->spec);
        books[event->spec] = book;
        book->onEvent(event);
        return;
    }

    it->second->onEvent(event);
}

MLB::Book* MLB::Books::get(const Spec* spec) const
{
    std::map<const Spec*, Book*>::const_iterator it = books.find(spec);

    if (it == books.end())
    {
        return 0;
    }

    return it->second;
}

int MLB::Books::size() const
{
    return books.size();
}
