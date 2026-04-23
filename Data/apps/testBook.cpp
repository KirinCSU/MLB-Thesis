#include <iostream>

#include "Book.h"
#include "BookEvent.h"
#include "Spec.h"
#include "Time.h"

using namespace MLB;

int main()
{
    Spec spec("AAPL", 0);
    Book book(&spec);

    Time t;
    t.sec = 9 * 3600 + 30 * 60;
    t.usec = 100;

    // ADD
    BookEvent* e1 = new BookEvent(0, t);
    e1->setAddition(&spec, 1001, true, 10, 150.25);
    book.onEvent(e1);

    // EXECUTE (partial)
    BookEvent* e2 = new BookEvent(0, t);
    e2->setExecution(&spec, 1001, 5);
    book.onEvent(e2);

    // DELETE
    BookEvent* e3 = new BookEvent(0, t);
    e3->setDeletion(&spec, 1001);
    book.onEvent(e3);

    std::cout << "Final size (should be 0): " << book.size() << std::endl;

    delete e1;
    delete e2;
    delete e3;

    return 0;
}
