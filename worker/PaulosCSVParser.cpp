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
#include <string>
#include <iostream>
#include <fstream>

#include "PaulosCSVParser.hpp"
#include "../commons/UCDLogger.hpp"

bool PaulosCSVParser::getDictionary(std::string file_name, std::unordered_map<std::string, std::string>& dict) const
{
    std::unordered_map<std::string, std::string> map;
    std::ifstream file(file_name);

    if (!file.is_open())
    {
        UCD_LOGGER(LOG_ERR, "File " + file_name + " not open");
        return false;
    }
    
    std::string line;
    while (std::getline(file, line))
    {
        parseAndAppendLine(line, dict);
        UCD_LOGGER(LOG_DEBUG, "Appending " + line);
    }
    
    return true;
}

inline void PaulosCSVParser::parseAndAppendLine(std::string line, std::unordered_map<std::string, std::string>& dict) const
{
    size_t start = 0, end = 0;
    bool is_key = true;
    std::string key, value;
    std::string delimiter = ",";
    const char delimiter2 = '\"';

    while((end = line.find(delimiter, start)) != std::string::npos) {
        if (is_key)
        {
            key = line.substr(start, end - start);
            is_key = false;
        } else {
            value = line.substr(start, end - start); 
            break;
        }
        start = end + delimiter.length();
        if (line[start] == delimiter2){
            delimiter = delimiter2;
            start += 1;
        }
    }
    dict[key] = value;
}



