#ifndef MLB_SPEC_H
#define MLB_SPEC_H

#include "Elements.h"

namespace MLB
{

class Spec
{
public:
    String ticker;
    int tickerID;

public:
    Spec(const String& ticker, int tickerID);
    ~Spec();
};

}

#endif
