#ifndef COMPRESSED_READER_H
#define COMPRESSED_READER_H

#include "Compressed.h"
#include "BookEvent.h"
#include "EventQueue.h"
#include "Publisher.h"
#include "Spec.h"
#include "Elements.h"

#include <cstdio>

namespace MLB
{

class CompressedReader : public Publisher
{
private:
    static const int MaxEntries = 4096;

private:
    int seconds;
    std::vector<const Spec*> tickerIDToSpec;
    Ints prices;

    int index;
    int maxIndex;
    BookEvent* head;
    BookEvent* tail;

private:
    EventQueue* eventQueue;
    FILE* file;
    Compressed::Entry buffer[MaxEntries];

public:
    CompressedReader(const char* filename, EventQueue* eventQueue);
    ~CompressedReader();

    void nextEvent();

private:
    void getNextRecord();
};

}

#endif
