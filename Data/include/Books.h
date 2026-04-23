#ifndef BOOKS_H
#define BOOKS_H

#include "Book.h"
#include "BookEvent.h"
#include "Spec.h"
#include "Subscriber.h"

#include <map>

namespace MLB
{

class Books : public Subscriber
{
private:
    std::map<const Spec*, Book*> books;

public:
    Books();
    ~Books();

    void onEvent(const BookEvent* event);

    Book* get(const Spec* spec) const;

    int size() const;
};

}

#endif
