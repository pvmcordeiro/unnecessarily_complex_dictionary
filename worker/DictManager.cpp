#include "DictManager.hpp"

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

bool DictManager::searchWord(std::string word, std::string& result) const {
    auto val = dict.find(word);
    if (dict.end() == val)
    {
        return false;
    }
    result = val->second;
    return true;
}

bool DictManager::searchAproxWord(std::string word, std::string& result) const {
    std::vector<std::string> keys;
    std::ostringstream osResult;

    for (const auto& pair : dict)
    {
        if (pair.first.find(word) != std::string::npos)
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
            osResult << '\t' + k << " = " << dict[k] << "\n";
        }
        result = osResult.str();
        return true;
    }
    return false;
}
