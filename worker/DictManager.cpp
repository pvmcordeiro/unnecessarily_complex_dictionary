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
