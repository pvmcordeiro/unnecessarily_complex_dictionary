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
#include "ProtocolParserServer.hpp"

/*
 *  Here are described all the callback functions that must be
 *  provided to the constructor 
*/

// TODO: remove this line after adding DB
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
        UCD_LOGGER(LOG_INFO, "Single translation for : " + wordToTranslate + " = " + translatedWord);
        response.command = UCDProtocol::Command::RESPONSE;
        response.format = UCDProtocol::PayloadFormat::STRING;
        response.payloadSize = translatedWord.size();
        response.payload.assign(translatedWord.begin(), translatedWord.end());

    } else if (dictManager.searchAproxWord(wordToTranslate, translatedWord)) 
    {
        UCD_LOGGER(LOG_INFO, "Multiples responses for: " + wordToTranslate);
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

    UCD_LOGGER(LOG_INFO, "Loading Dictionary from Paulo");

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
ProtocolParserServer::~ProtocolParserServer() {}

/**
 * This is the constructor of the ProtocolParserServer class,
 * whenever a new protocol map is inserted it must be added as
 * a lambda function in the vector declaration
 */
ProtocolParserServer::ProtocolParserServer()
{
    rootProtocolSlices = {
        []{
            auto myComparison = [](const UCDPackage &pkg) -> bool {
                return (pkg.command == UCDProtocol::Command::SEARCH);
            };

            auto slice = std::make_shared<UCDProtocolSlice>(myComparison);
            UCDProtocol::Command cmd = UCDProtocol::Command::SEARCH;
            //slice->addSlice(cmd)->assignCallback(v1_search_string_cb);
            slice->assignCallback(v1_search_string_cb);
            return slice;
        }(),
        []{
            auto myComparison = [](const UCDPackage &pkg) -> bool {
                return (pkg.command == UCDProtocol::Command::LOAD_DICT_FROM_PAULO_CSV);
            };
            auto slice = std::make_shared<UCDProtocolSlice>(myComparison);
            UCDProtocol::Command cmd = UCDProtocol::Command::LOAD_DICT_FROM_PAULO_CSV;
            //slice->addSlice(cmd)->assignCallback(v1_load_from_file);
            slice->assignCallback(v1_load_from_file);
            return slice;
        }()
    };
}

UCDPackage ProtocolParserServer::processMsg(const UCDPackage& req)
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
