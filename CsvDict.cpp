#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <sstream>
#include "CsvDict.hpp"

std::map<std::string,std::string> CsvDict::dict;

CsvDict::~CsvDict() {}

CsvDict::CsvDict() {}

void CsvDict::loadDictFromCsvFile()
{
    std::ifstream file("dict-dutch-pt.csv");

    if ( !file.is_open()) {
        std::cerr << "Failed to open csn dict" << std::endl;
    }

    std::string line;
    while (std::getline(file,line))
    {
        appendDictMap(line);
    }
}

void CsvDict::appendDictMap(const std::string& _line)
{  
    size_t start = 0, end = 0;
    bool is_key = true;
    std::string key, value;
    std::string delimiter = ",";
    char delimiter2 = '\"';

    while((end = _line.find(delimiter, start)) != std::string::npos) {
        if (is_key)
        {
            key = _line.substr(start, end - start);
            is_key = false;
        } else {
            value = _line.substr(start, end - start); 
            break;
        }
        start = end + delimiter.length();
        if (_line[start] == delimiter2){
            delimiter = delimiter2;
            start += 1;
        }
    }
    dict[key] = value;
}

std::string CsvDict::search(const std::string _key)
{
    return dict[_key];
}

std::string CsvDict::searchAprox(const std::string _key)
{
    std::vector<std::string> keys;
    std::ostringstream result;

    for (const auto& pair : dict)
    {
        if (pair.first.find(_key) != std::string::npos)
        {
            if (!pair.second.empty())
            {
                keys.push_back(pair.first);
            }
        }
}
    if (keys.size() > 0)
    {
        for ( const auto& k : keys)
        {
            result << '\t' << k << " = " << dict[k] << "\n";
        }
    }
    return result.str();
}

void CsvDict::printDict()
{
    for (auto it = dict.begin(); it != dict.end(); ++it)
    {
        std::cout << it->first << " = " << it->second << std::endl;
    }
}