#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <vector>

namespace MLB
{

class Publisher
{
public:
    virtual ~Publisher() {}
    virtual void nextEvent() {}
};

typedef std::vector<Publisher*> Publishers;

}

#endif
