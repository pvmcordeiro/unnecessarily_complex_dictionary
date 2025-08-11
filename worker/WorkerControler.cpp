#include "WorkerControler.hpp"

#include <iostream>
#include <thread>
#include <string>

#include "../commons/UCDLogger.hpp"
#include "../worker/DictManager.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

UCDPackage WorkerControler::processReceivedMsg(const boost::beast::flat_buffer& buff)
{
    std::string msg = beast::buffers_to_string(buff.data());
    boost::json::value jv = boost::json::parse(msg);

    UCDPackage response;
    DictManager dictManager = {myDict};

    UCDPackage pack;
    pack.deserializeUCDPackage(jv);
    if (pack.command == CommandFactory::Command::SEARCH)
    {
        if (pack.format == CommandFactory::PayloadFormat::STRING)
        {
            std::string wordToTranslate(pack.payload.begin(), pack.payload.begin() + pack.payloadSize);

            std::string translatedWord;

            UCD_LOGGER(LOG_INFO, "Received => " + wordToTranslate);

            if (dictManager.searchWord(wordToTranslate, translatedWord))
            {
                response.command = CommandFactory::Command::RESPONSE;
                response.format = CommandFactory::PayloadFormat::STRING;
                response.payloadSize = translatedWord.size();
                response.payload.assign(translatedWord.begin(), translatedWord.end());
            } else {
                if (!dictManager.searchAproxWord(wordToTranslate, translatedWord))
                {
                    response.command = CommandFactory::Command::RESPONSE;
                    response.format = CommandFactory::PayloadFormat::STRING;
                    response.payloadSize = translatedWord.size();
                    response.payload.assign(translatedWord.begin(), translatedWord.end());
                } else
                {
                    UCD_LOGGER(LOG_INFO, "Word '" + wordToTranslate + "' not found");
                }
            }
        }
    }
    return response;
}

void WorkerControler::websocket_worker(tcp::socket socket) {

    try {
        websocket::stream<tcp::socket> ws(std::move(socket));
        ws.accept();

        for (;;) {
            beast::flat_buffer buffer;
            ws.read(buffer);

            auto resp = processReceivedMsg(buffer);

            ws.text(ws.got_text());

            UCD_LOGGER(LOG_INFO, "Responding <= ");
            ws.write(asio::buffer(boost::json::serialize(resp.serializeUCDPackage())));
        }
    } catch (const beast::system_error& se) {
        UCD_LOGGER(LOG_ERR, "WebSocket error: " + std::string(se.what()));
    }
}

bool WorkerControler::startConnectionThread()
{

    // TODO remove this hardcoded file
    std::string dictFileName = "dict-dutch-pt.csv";
    
    if (! cvsParser.getDictionary(dictFileName, myDict))
    {
        UCD_LOGGER(LOG_ERR, "Not possible to parse dictionary from " + dictFileName);
        return 1;
    }

    asio::io_context ioc;
    tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 9002));

    UCD_LOGGER(LOG_INFO, "WebSocket server listening on port 9002...");

    for (;;) {
        tcp::socket socket(ioc);
        acceptor.accept(socket);
        std::thread{&WorkerControler::websocket_worker, this, std::move(socket)}.detach();
    }
    
    return 0;
}

