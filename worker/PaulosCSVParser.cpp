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
        UCDLogger::getInstance()->log(LOG_ERR, "File " + file_name + " not open");
        return false;
    }
    
    std::string line;
    while (std::getline(file, line))
    {
        parseAndAppendLine(line, dict);
        UCDLogger::getInstance()->log(LOG_DEBUG, "Appending " + line);
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



