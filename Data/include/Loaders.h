#ifndef LOADERS_H
#define LOADERS_H

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Elements.h"

namespace MLB
{
    typedef std::map<String, double> Vols;

    Vols loadVols(String bodFile)
    {
        Vols vols;

        std::ifstream file(bodFile);

        if(!file.is_open())
        {
            std::cerr << "Failed to open file!" << std::endl;
            return vols;
        }

        String line;

        while(std::getline(file, line))
        {

            double vol5 = 0.0;

            std::istringstream iss(line);
            String ticker;
            iss >> ticker;

            String key = "Volatility5|";
            size_t start = line.find(key);

            if(start != std::string::npos)
            {
                start += key.size();
                size_t end = line.find('|', start);

                vol5 = std::stod(line.substr(start, end - start));
            }

            vols[ticker] = vol5;
        }

        file.close();

        return vols;
    }
}

#endif
