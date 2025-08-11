#pragma once

#include <string>
#include <boost/json.hpp>
#include "../commons/UCDProtocol.hpp"

class ClientCommand {
    boost::json::object requestWord(std::string);
    bool loadNewDictionary(std::string);
};

class TranslateWord : public UCDProtocol {
    private:
        UCDPackage pack;
    public:
    TranslateWord(std::string word) {
        pack.command = UCDProtocol::Command::SEARCH;
        pack.format = UCDProtocol::PayloadFormat::STRING;
        pack.payload.assign(word.begin(), word.end());
        pack.payloadSize = word.size();
    };
    ~TranslateWord() = default;
};
