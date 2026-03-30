#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <sys/types.h>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <deque>
#include <set>

namespace MLB
{
    typedef int8_t Int8;
    typedef int16_t Int16;
    typedef int32_t Int32;
    typedef int64_t Int64;

    typedef uint8_t UInt8;
    typedef uint16_t UInt16;
    typedef uint32_t UInt32;
    typedef uint64_t UInt64;

    static const bool Buy = true;
    static const bool Sell = false;

    typedef std::string String;

    //Useful Containers

    typedef std::pair<int, int>        PairII;
    typedef std::pair<int, bool>       PairIB;
    typedef std::pair<int, double>     PairID;
    typedef std::pair<int, String>     PairIS;
    typedef std::pair<bool, int>       PairBI;
    typedef std::pair<bool, bool>      PairBB;
    typedef std::pair<bool, double>    PairBD;
    typedef std::pair<bool, String>    PairBS;
    typedef std::pair<double, int>     PairDI;
    typedef std::pair<double, bool>    PairDB;
    typedef std::pair<double, double>  PairDD;
    typedef std::pair<double, String>  PairDS;
    typedef std::pair<String, int>     PairSI;
    typedef std::pair<String, bool>    PairSB;
    typedef std::pair<String, double>  PairSD;
    typedef std::pair<String, String>  PairSS;

    typedef std::vector<int>     Ints;
    typedef std::vector<bool>    Bools;
    typedef std::vector<char>    Chars;
    typedef std::vector<float>   Floats;
    typedef std::vector<double>  Doubles;
    typedef std::vector<String>  Strings;
    typedef std::vector<PairID>  PairsID;
    typedef std::vector<PairII>  PairsII;
    typedef std::vector<PairIS>  PairsIS;
    typedef std::vector<PairSB>  PairsSB;
    typedef std::vector<PairSD>  PairsSD;
    typedef std::vector<PairSI>  PairsSI;
    typedef std::vector<PairSS>  PairsSS;
    typedef std::vector<PairDB>  PairsDB;
    typedef std::vector<PairDD>  PairsDD;

    typedef Ints    VecI;
    typedef Bools   VecB;
    typedef Chars   VecC;
    typedef Floats  VecF;
    typedef Doubles VecD;
    typedef Strings VecS;
    typedef PairsID VecID;
    typedef PairsII VecII;
    typedef PairsIS VecIS;
    typedef PairsSB VecSB;
    typedef PairsSD VecSD;
    typedef PairsSI VecSI;
    typedef PairsSS VecSS;
    typedef PairsDB VecDB;
    typedef PairsDD VecDD;

    typedef std::deque<int>     DeqI;
    typedef std::deque<bool>    DeqB;
    typedef std::deque<char>    DeqC;
    typedef std::deque<double>  DeqD;
    typedef std::deque<String>  DeqS;
    typedef std::deque<PairII>  DeqII;
    typedef std::deque<PairID>  DeqID;
    typedef std::deque<PairIS>  DeqIS;
    typedef std::deque<PairSI>  DeqSI;
    typedef std::deque<PairSD>  DeqSD;
    typedef std::deque<PairSS>  DeqSS;
    typedef std::deque<PairDD>  DeqDD;

    typedef std::map<int, int>        MapII;
    typedef std::map<int, bool>       MapIB;
    typedef std::map<int, double>     MapID;
    typedef std::map<int, String>     MapIS;
    typedef std::map<String, int>     MapSI;
    typedef std::map<String, char>    MapSC;
    typedef std::map<String, bool>    MapSB;
    typedef std::map<String, double>  MapSD;
    typedef std::map<String, String>  MapSS;
    typedef std::map<String, Doubles> MapSDbls;
    typedef std::map<String, VecD>    MapSVecD;

    typedef std::multimap<int, int>        MMapII;
    typedef std::multimap<int, bool>       MMapIB;
    typedef std::multimap<int, double>     MMapID;
    typedef std::multimap<int, String>     MMapIS;
    typedef std::multimap<String, int>     MMapSI;
    typedef std::multimap<String, bool>    MMapSB;
    typedef std::multimap<String, double>  MMapSD;
    typedef std::multimap<String, String>  MMapSS;
    typedef std::multimap<String, Doubles> MMapSDbls;

    typedef std::set<int>     SetI;
    typedef std::set<UInt32>  SetU;
    typedef std::set<bool>    SetB;
    typedef std::set<char>    SetC;
    typedef std::set<String>  SetS;

    typedef std::set<PairII>  SetII;
    typedef std::set<PairSS>  SetSS;

    typedef std::multiset<int>     MSetI;
    typedef std::multiset<bool>    MSetB;
    typedef std::multiset<char>    MSetC;
    typedef std::multiset<String>  MSetS;

    typedef std::vector<VecB>    VecVecB;
    typedef std::vector<VecD>    VecVecD;
    typedef std::vector<VecI>    VecVecI;
    typedef std::vector<VecII>   VecVecII;
    typedef std::vector<VecID>   VecVecID;
    typedef std::vector<DeqD>    VecDeqD;
    typedef std::vector<DeqI>    VecDeqI;
    typedef std::vector<VecVecD> VecVecVecD;
    typedef std::vector<VecVecI> VecVecVecI;
    typedef std::vector<VecDeqD> VecVecDeqD;
}

#endif
