#include <iostream>

#include "Books.h"
#include "BookEvent.h"
#include "Spec.h"
#include "Time.h"

using namespace MLB;

int main()
{
    Spec specA("AAPL", 0);
    Spec specB("MSFT", 1);

    Books books;

    Time t;
    t.sec = 9 * 3600 + 30 * 60;
    t.usec = 100;

    BookEvent* e1 = new BookEvent(0, t);
    e1->setAddition(&specA, 1001, true, 10, 150.25);
    books.onEvent(e1);

    BookEvent* e2 = new BookEvent(0, t);
    e2->setAddition(&specB, 2001, false, 20, 300.50);
    books.onEvent(e2);

    BookEvent* e3 = new BookEvent(0, t);
    e3->setExecution(&specA, 1001, 5);
    books.onEvent(e3);

    Book* bookA = books.get(&specA);
    Book* bookB = books.get(&specB);

    std::cout << "Books size (should be 2): " << books.size() << std::endl;

    if (bookA)
    {
        std::cout << "AAPL size (should be 1): " << bookA->size() << std::endl;
    }
    else
    {
        std::cout << "AAPL book missing" << std::endl;
    }

    if (bookB)
    {
        std::cout << "MSFT size (should be 1): " << bookB->size() << std::endl;
    }
    else
    {
        std::cout << "MSFT book missing" << std::endl;
    }

    delete e1;
    delete e2;
    delete e3;

    return 0;
}
