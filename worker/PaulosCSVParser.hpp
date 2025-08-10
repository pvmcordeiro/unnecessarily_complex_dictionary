#pragma once
#include "LocalFilesInterface.hpp"


class PaulosCSVParser : public LocalFilesInterface {
public:
    PaulosCSVParser() {};
    ~PaulosCSVParser() {};
    bool getDictionary(std::string, std::unordered_map<std::string, std::string> &) const override;
private:
    inline void parseAndAppendLine(std::string, std::unordered_map<std::string, std::string>&) const;
};