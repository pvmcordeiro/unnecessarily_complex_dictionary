#pragma once

#include <string>
#include <boost/json.hpp>
#include "../commons/CommandFactory.hpp"

class ClientCommand {
    boost::json::object requestWord(std::string);
    bool loadNewDictionary(std::string);
};

class TranslateWord : public CommandFactory {
    private:
        UCDPackage pack;
    public:
    TranslateWord(std::string word) {
        pack.command = CommandFactory::Command::SEARCH;
        pack.format = CommandFactory::PayloadFormat::STRING;
        pack.payload.assign(word.begin(), word.end());
        pack.payloadSize = word.size();
    };
    ~TranslateWord() = default;
};
