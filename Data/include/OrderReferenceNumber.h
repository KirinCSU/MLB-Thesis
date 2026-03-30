#ifndef ORDER_REFERENCE_NUMBER_H
#define ORDER_REFERENCE_NUMBER_H

#include <set>
#include "Elements.h"
#include "Helpers.h"

namespace MLB
{

    typedef UInt64 OrderReferenceNumber;
    typedef std::set<OrderReferenceNumber> SetOrderReferenceNumbers;

    inline OrderReferenceNumber ParseORN(const char* buffer, int offset, int length)
    {
        OrderReferenceNumber result = 0;
        int i = offset;

        while(i < offset + length)
        {
            char c = buffer[i];
            if(IsNonZeroDigit(c))
            {
                break;
            }
            i++;
        }

        while(i < offset + length)
        {
            char c = buffer[i];
            if(!IsDigit(c))
            {
                break;
            }

            result *= 10;
            result += (c - 48);
            i++;
        }

        return result;
    }
}

#endif
