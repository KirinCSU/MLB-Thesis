#include <iostream>

#include "Books.h"
#include "CompressedReader.h"
#include "EventQueue.h"

using namespace MLB;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <compressed_file>" << std::endl;
        return 1;
    }

    EventQueue queue;
    Books books;
    queue.addSubscriber(&books);

    CompressedReader reader(argv[1], &queue);

    while (queue.nextEvent())
    {
    }

    std::cout << "Finished full pipeline run" << std::endl;
    std::cout << "Books created: " << books.size() << std::endl;

    return 0;
}
