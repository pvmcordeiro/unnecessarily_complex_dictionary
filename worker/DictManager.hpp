#pragma once
#include <string>
#include <unordered_map>

class DictManager {

private:
    std::unordered_map<std::string, std::string>& dict;
public:
    DictManager(std::unordered_map<std::string, std::string>& dictRef)
        : dict(dictRef)
    {
    };
    ~DictManager() = default;
    bool searchWord(std::string, std::string&) const;
    bool searchAproxWord(std::string, std::string&) const;
};