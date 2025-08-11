/*
MIT License

Copyright (c) 2025 Paulo Cordeiro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
    // TODO: remove hardcoded file
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