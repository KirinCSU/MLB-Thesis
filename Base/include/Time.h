#ifndef TIME_H
#define TIME_H

#include "Elements.h"
#include <algorithm>
#include <cstdio>

namespace MLB
{
    static const int OneDaySecs = 24 * 3600;

    class Time
    {
        public:

            int sec;
            int usec;

            Time() : sec(0), usec(0)
            {
            }

            explicit Time(UInt64 usecFromMidnight)
            {
                usec = usecFromMidnight % 1000000;
                sec = (usecFromMidnight - usec) / 1000000;
            }

            String str(bool pretty = false) const
            {
                char buffer[40];

                int adjU = std::max(0, usec);
                int adjS = std::max(0, sec);

                if (adjS < OneDaySecs)
                {
                    int ss = adjS % 60;
                    int hh = adjS / 3600;
                    int mm = (adjS - hh * 3600) / 60;

                    if (!pretty)
                    {
                        sprintf(buffer, "%02d%02d%02d.%06d", hh, mm, ss, adjU);
                    }
                    else
                    {
                        sprintf(buffer, "%02d:%02d:%02d.%06d", hh, mm, ss, adjU);
                    }
                }
                else
                {
                    int ddd = adjS / OneDaySecs;
                    adjS -= ddd * OneDaySecs;

                    int ss = adjS % 60;
                    int hh = adjS / 3600;
                    int mm = (adjS - hh * 3600) / 60;

                    sprintf(buffer, "L%03d:%02d:%02d:%02d", ddd, hh, mm, ss);
                }

                return buffer;
            }
    };
}

#endif
