#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

class LocalFilesInterface {
public:
    virtual ~LocalFilesInterface() = default;

    // Pure virtual method to get a dictionary
    virtual bool getDictionary(std::string, std::unordered_map<std::string, std::string> &) const = 0;
};