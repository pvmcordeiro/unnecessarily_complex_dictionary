
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "../commons/UCDProtocol.hpp"
#include "../commons/UCDLogger.hpp"
#include "DictManager.hpp"
#include "PaulosCSVParser.hpp"

std::unordered_map<std::string, std::string> myDict;

UCDPackage v1_search_string_cb(const unsigned int& size, const std::vector<char>& payload)
{
    std::string wordToTranslate(payload.begin(), payload.begin() + size);
    std::string translatedWord;
    DictManager dictManager = {myDict};

    UCDPackage response;

    UCD_LOGGER(LOG_INFO, "Received => " + wordToTranslate);

    if (dictManager.searchWord(wordToTranslate, translatedWord))
    {
        response.command = UCDProtocol::Command::RESPONSE;
        response.format = UCDProtocol::PayloadFormat::STRING;
        response.payloadSize = translatedWord.size();
        response.payload.assign(translatedWord.begin(), translatedWord.end());
    } else if (dictManager.searchAproxWord(wordToTranslate, translatedWord)) 
    {
        response.command = UCDProtocol::Command::RESPONSE;
        response.format = UCDProtocol::PayloadFormat::STRING;
        response.payloadSize = translatedWord.size();
        response.payload.assign(translatedWord.begin(), translatedWord.end());
    } else
    {
        UCD_LOGGER(LOG_INFO, "Word '" + wordToTranslate + "' not found");
    }
    return response;
}

UCDPackage v1_load_from_file(const unsigned int& size, const std::vector<char>& payload)
{
    // ----- THIS BLOCK IS ONLY TEMPORARY ------
    // TODO remove this hardcoded file
    std::string dictFileName = "dict-dutch-pt.csv";
    UCDPackage response;
    std::string translatedWord = "LOADED";
    response.command = UCDProtocol::Command::RESPONSE;
    response.format = UCDProtocol::PayloadFormat::STRING;
    response.payloadSize = translatedWord.size();
    response.payload.assign(translatedWord.begin(), translatedWord.end());

    PaulosCSVParser cvsParser;
    if (! cvsParser.getDictionary(dictFileName, myDict))
    {
        UCD_LOGGER(LOG_ERR, "Not possible to parse dictionary from " + dictFileName);
    }
    return response;
}

class ProtocolParserForServer {
    private:
    const std::vector<std::shared_ptr<UCDProtocolSlice>> rootProtocolSlices = {
    []{
        auto slice = std::make_shared<UCDProtocolSlice>(UCD_PROTOCOL_CURRENT_VERSION)->addSlice(UCDProtocol::Command::SEARCH);
        slice->assignCallback(v1_search_string_cb);
        return slice;
    }(),
    []{
        auto slice = std::make_shared<UCDProtocolSlice>(UCD_PROTOCOL_CURRENT_VERSION)->addSlice(UCDProtocol::Command::LOAD_DICT_FROM_PAULO_CSV);
        slice->assignCallback(v1_load_from_file);
        return slice;
    }()
};

    public:
    ProtocolParserForServer()
    {
    }

    UCDPackage processMsg(const UCDPackage& req)
    {
        UCDPackage resp;
        resp.response = UCDProtocol::Response::FAIL;

        for (auto it = rootProtocolSlices.begin(); it != rootProtocolSlices.end(); it++)
        {
            if ((*it)->execCallbackInTree(req, resp))
            {
                return resp;
            }
        }
        return resp;
    }
};