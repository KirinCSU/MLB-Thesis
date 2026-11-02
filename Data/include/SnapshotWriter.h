#ifndef SNAPSHOT_WRITER_H
#define SNAPSHOT_WRITER_H

#include "Book.h"

#include <map>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <cstdint>
#include <vector>
#include <set>

namespace MLB
{

    struct PriceLevel
    {
        double orders; //# of orders at price level
        double volume; //Total volume at price level
        double fraction; //% of volume at price level out of volume across all price levels
    };

    typedef std::vector<PriceLevel> VecLevel;

    class Snapshot
    {
        int levels;
        String ticker;

        VecLevel bids; //bid-(n-1), ..., bid-0
        VecLevel asks; //ask+0, ..., ask+(n-1)

        double change;
        double mid;
        double lastMid;

        public:
        Snapshot(const String& pTicker, int n)
        {
            levels = n;
            ticker = pTicker;
            change = 0;
            mid = 0;
            lastMid = 0;

            for(int i = 0; i < n; i++)
            {
                PriceLevel level;
                level.orders = 0;
                level.volume = 0;
                level.fraction = 0;

                bids.push_back(level);
                asks.push_back(level);
            }

            return;
        }

        void printSnapshot()
        {
            std::cout << ticker << ", ";

            for(int i = 0; i < bids.size(); i++)
            {
                std::cout << bids[i].fraction << ", ";
            }

            for(int i = 0; i < asks.size(); i++)
            {
                std::cout << asks[i].fraction << ", ";
            }

            std::cout << change << std::endl;

            return;
        }

        void update(const Book& book)
        {
            const auto& bookBids = book.getBids();
            const auto& bookAsks = book.getAsks();

            for(int i = 0; i < levels; i++)
            {
                bids[i].orders = 0;
                bids[i].volume = 0;
                bids[i].fraction = 0;

                asks[i].orders = 0;
                asks[i].volume = 0;
                asks[i].fraction = 0;
            }

            double totalVolume = 0;
            double bestBid = 0;
            double bestAsk = 0;

            int bidIndex = levels - 1;

            for(auto bidIt = bookBids.rbegin();
                    bidIt != bookBids.rend() && bidIndex >= 0;
                    ++bidIt)
            {
                if(bidIt->second.empty())
                    continue;

                if(bestBid == 0)
                    bestBid = bidIt->first;

                BookEntry* entry = bidIt->second.front();

                while(entry != nullptr)
                {
                    bids[bidIndex].orders++;
                    bids[bidIndex].volume += entry->size;
                    entry = entry->next;
                }

                totalVolume += bids[bidIndex].volume;
                bidIndex--;
            }

            int askIndex = 0;

            for(auto askIt = bookAsks.begin();
                    askIt != bookAsks.end() && askIndex < levels;
                    ++askIt)
            {
                if(askIt->second.empty())
                    continue;

                if(bestAsk == 0)
                    bestAsk = askIt->first;

                BookEntry* entry = askIt->second.front();

                while(entry != nullptr)
                {
                    asks[askIndex].orders++;
                    asks[askIndex].volume += entry->size;
                    entry = entry->next;
                }

                totalVolume += asks[askIndex].volume;
                askIndex++;
            }

            if(totalVolume == 0)
                return;

            for(int i = 0; i < levels; i++)
            {
                bids[i].fraction = bids[i].volume / totalVolume;
                asks[i].fraction = asks[i].volume / totalVolume;
            }

            if(bestBid == 0 || bestAsk == 0)
                return;

            lastMid = mid;
            mid = (bestBid + bestAsk) / 2.0;

            if(lastMid != 0)
                change = (mid - lastMid) / lastMid;
            else
                change = 0;
        }

        /*
           void update(const Book& book)
        {
            const auto& bookBids = book.getBids();
            const auto& bookAsks = book.getAsks();

            auto bidItT = bookBids.rbegin();

            std::cout << "Best bid key: " << bidItT->first
                << " empty: " << bidItT->second.empty()
                << std::endl;

            auto askItT = bookAsks.begin();

            std::cout << "Best ask key: " << askItT->first
                << " empty: " << askItT->second.empty()
                << std::endl;

            for(int i = 0; i < levels; i++)
            {
                bids[i].orders = 0;
                bids[i].volume = 0;
                bids[i].fraction = 0;

                asks[i].orders = 0;
                asks[i].volume = 0;
                asks[i].fraction = 0;
            }

            double totalVolume = 0;

            auto bidIt = bookBids.rbegin();

            for(int i = levels - 1;
                    i >= 0 && bidIt != bookBids.rend();
                    i--, bidIt++)
            {
                BookEntry* entry = bidIt->second.front();

                while(entry != nullptr)
                {
                    bids[i].orders++;
                    bids[i].volume += entry->size;
                    entry = entry->next;
                }

                totalVolume += bids[i].volume;
            }

            auto askIt = bookAsks.begin();

            for(int i = 0;
                    i < levels && askIt != bookAsks.end();
                    i++, askIt++)
            {
                BookEntry* entry = askIt->second.front();

                while(entry != nullptr)
                {
                    asks[i].orders++;
                    asks[i].volume += entry->size;
                    entry = entry->next;
                }

                totalVolume += asks[i].volume;
            }

            if(totalVolume == 0)
                return;

            for(int i = 0; i < levels; i++)
            {
                bids[i].fraction = bids[i].volume / totalVolume;
                asks[i].fraction = asks[i].volume / totalVolume;
            }

            lastMid = mid;

            if(!bookBids.empty() && !bookAsks.empty())
            {
                double bestBid = bookBids.rbegin()->first;
                double bestAsk = bookAsks.begin()->first;

                mid = (bestBid + bestAsk) / 2.0;
            }

            if(lastMid != 0)
                change = (mid - lastMid) / lastMid;
            else
                change = 0;
        }
    */

        void write(std::ostream& out)
        {
            printSnapshot();
        }
    };

    struct SnapshotEntry
    {
        const Spec* spec;
        Snapshot* snapshot;
        std::ofstream* file;
    };

    typedef std::map<String, SnapshotEntry> Entries;

    class SnapshotWriter
    {
        private:
            Books* books;
            Entries entries;

        public:
            SnapshotWriter(Books* pBooks, const Strings& pTickers, int pLevels)
            {
                books = pBooks;

                for(const String& ticker : pTickers)
                {
                    SnapshotEntry entry;

                    entry.spec = nullptr;
                    entry.snapshot = new Snapshot(ticker, pLevels);
                    entry.file = nullptr;

                    entries[ticker] = entry;
                }
            }

            void link(const Spec* spec)
            {
                auto it = entries.find(spec->ticker);

                if(it == entries.end())
                {
                    return;
                }

                if(it->second.spec == nullptr)
                {
                    it->second.spec = spec;
                }
            }

            void status()
            {
                for(const auto& entry : entries)
                {

                    if(entry.second.spec == nullptr)
                    {
                        std::cout << "Missing Spec for: " << entry.first << std::endl;
                        continue;
                    }

                    std::cout << entry.first << " " << entry.second.spec->ticker << std::endl;
                }
            }

            void update()
            {
                for(auto& item : entries)
                {
                    SnapshotEntry& entry = item.second;

                    if(entry.spec == nullptr)
                    {
                        std::cout << "Entry.spec NULL" << std::endl;
                        continue;
                    }

                    Book* book = books->get(entry.spec);

                    if(book == nullptr)
                    {
                        std::cout << "Book NULL" << std::endl;
                        continue;
                    }

                    entry.snapshot->update(*book);
                }
            }

            void write()
            {
                for(auto& item : entries)
                {
                    SnapshotEntry& entry = item.second;

                    if(entry.spec == nullptr)
                    {
                        continue;
                    }

                    entry.snapshot->printSnapshot();
                }
            }
    };
}

#endif
