#ifndef DAILY_BARS_H
#define DAILY_BARS_H

#include "Elements.h"

#include <fstream>
#include <algorithm>

namespace MLB
{

class DailyBars
{
public:
    struct Row
    {
        String ticker;
        double open;
        double high;
        double low;
        double close;
        Int64 volume;
        double vwap;
    };

private:
    std::vector<Row> rows;

public:
    DailyBars(const char* filename)
    {
        read(filename);
    }

    void read(const char* filename)
    {
        std::ifstream in(filename);

        if (!in)
        {
            ERROR(String("Unable to open daily bars file: ") + filename);
        }

        String header;
        getline(in, header);

        Row row;
        while (in >> row.ticker
                  >> row.open
                  >> row.high
                  >> row.low
                  >> row.close
                  >> row.volume
                  >> row.vwap)
        {
            rows.push_back(row);
        }
    }

    Strings topNLiquid(int n) const
    {
        std::vector<Row> sorted = rows;

        std::sort(sorted.begin(), sorted.end(),
                  [](const Row& a, const Row& b)
                  {
                      return (a.volume * a.vwap) > (b.volume * b.vwap);
                  });

        Strings result;

        for (int i = 0; i < n && i < (int)sorted.size(); i++)
        {
            result.push_back(sorted[i].ticker);
        }

        return result;
    }

    int size() const
    {
        return rows.size();
    }
};

}

#endif
