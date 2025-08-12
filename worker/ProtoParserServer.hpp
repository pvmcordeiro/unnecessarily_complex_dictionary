
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "../commons/UCDProtocol.hpp"
#include "../commons/UCDLogger.hpp"
#include "DictManager.hpp"

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
    } else {
        if (!dictManager.searchAproxWord(wordToTranslate, translatedWord))
        {
            response.command = UCDProtocol::Command::RESPONSE;
            response.format = UCDProtocol::PayloadFormat::STRING;
            response.payloadSize = translatedWord.size();
            response.payload.assign(translatedWord.begin(), translatedWord.end());
        } else
        {
            UCD_LOGGER(LOG_INFO, "Word '" + wordToTranslate + "' not found");
        }
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