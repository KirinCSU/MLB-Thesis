#ifndef TIME_H
#define TIME_H

#include "Elements.h"
#include <algorithm>
#include <cstdio>
#include "Helpers.h"

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

            Time(const String& str)
            {
                int hh = 0;
                int mm = 0;
                int ss = 0;

                if (str.size() == 13)
                {
                    // LDDD:HH:MM:SS - Large
                    if (str[0] == 'L')
                    {
                        int ddd;
                        sscanf(str.c_str(), "L%03d:%02d:%02d:%02d", &ddd, &hh, &mm, &ss);

                        usec = 0;
                        sec = 3600 * hh + 60 * mm + ss;
                        sec += ddd * OneDaySecs;
                    }
                    // HHMMSS.xxxyyy
                    else
                    {
                        sscanf(str.c_str(), "%02d%02d%02d.%06d", &hh, &mm, &ss, &usec);
                        sec = 3600 * hh + 60 * mm + ss;
                    }
                }
                // HH:MM:SS.xxxyyy
                else if (str.size() == 15)
                {
                    sscanf(str.c_str(), "%02d:%02d:%02d.%06d", &hh, &mm, &ss, &usec);
                    sec = 3600 * hh + 60 * mm + ss;
                }
                // HH:MM:SS
                else if (str.size() == 8)
                {
                    sscanf(str.c_str(), "%02d:%02d:%02d", &hh, &mm, &ss);
                    usec = 0;
                    sec = 3600 * hh + 60 * mm + ss;
                }
                // HHMMSS - Compact
                else if (str.size() == 6)
                {
                    sscanf(str.c_str(), "%02d%02d%02d", &hh, &mm, &ss);
                    usec = 0;
                    sec = 3600 * hh + 60 * mm + ss;
                }
                // HH:MM
                else if (str.size() == 5)
                {
                    sscanf(str.c_str(), "%02d:%02d", &hh, &mm);
                    usec = 0;
                    sec = 3600 * hh + 60 * mm;
                }
                else
                {
                    ERROR("Invalid Timestamp: " + str);
                }

            }

            /*
            Time& operator+=(const Offset& rhs)
            {
                if (rhs.increment)
                {
                    (*this) += rhs.offset;
                }
                else
                {
                    (*this) -= rhs.offset;
                }

                return *this;
            }
            */

            Time& operator+=(const Time& rhs)
            {
                sec += rhs.sec;
                usec += rhs.usec;

                if (usec >= 1000000)
                {
                    sec++;
                    usec -= 1000000;
                }

                return *this;
            }

            Time& operator-=(const Time& rhs)
            {
                sec -= rhs.sec;
                usec -= rhs.usec;

                if (usec < 0)
                {
                    sec--;
                    usec += 1000000;
                }

                return *this;
            }

            Time operator-(const Time& rhs) const
            {
                Time result(*this);
                result -= rhs;
                return result;
            }

            Time operator+(const Time& rhs) const
            {
                Time result(*this);
                result += rhs;
                return result;
            }

            bool operator< (const Time& rhs) const
            {
                return (sec < rhs.sec || (sec == rhs.sec && usec < rhs.usec));
            }

            bool operator<=(const Time& rhs) const
            {
                return !(*this > rhs);
            }

            bool operator> (const Time& rhs) const
            {
                return (sec > rhs.sec || (sec == rhs.sec && usec > rhs.usec));
            }

            bool operator>=(const Time& rhs) const
            {
                return !(*this < rhs);
            }

            bool operator==(const Time& rhs) const
            {
                return (sec == rhs.sec && usec == rhs.usec);
            }

            bool operator!=(const Time& rhs) const
            {
                return (sec != rhs.sec || usec != rhs.usec);
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
