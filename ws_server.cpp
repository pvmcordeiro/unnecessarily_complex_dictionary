#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <string>

#include "worker/CsvDict.hpp"
#include "worker/PaulosCSVParser.hpp"
#include "worker/DictManager.hpp"
#include "commons/UCDLogger.hpp"


namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

std::unordered_map<std::string, std::string> myDict;

void websocket_worker(tcp::socket socket) {
    std::string word;
    std::string translatedWord;
    DictManager dictManager = {myDict};

    try {
        websocket::stream<tcp::socket> ws(std::move(socket));
        ws.accept();

        for (;;) {
            beast::flat_buffer buffer;
            ws.read(buffer);
            std::string msg = beast::buffers_to_string(buffer.data());
            translatedWord.clear();

            UCDLogger::getInstance()->log(LOG_INFO, "Received => " + msg);

            if (!(msg.compare("exit") && msg.compare("quit")))
            {
                break;
            }
            if (!dictManager.searchWord(msg, translatedWord))
            {
                if (!dictManager.searchAproxWord(msg, translatedWord))
                {
                    UCDLogger::getInstance()->log(LOG_INFO, "Word '" + msg + "' not found");
                }
            }

            ws.text(ws.got_text());

            UCDLogger::getInstance()->log(LOG_INFO, "Responding <= " + translatedWord);
            ws.write(asio::buffer(translatedWord));
        }
    } catch (const beast::system_error& se) {
        UCDLogger::getInstance()->log(LOG_ERR, "WebSocket error: " + std::string(se.what()));
    }
}

int main() {
    try {
        UCDLogger::getInstance()->enableStdoutPrint(true);
        UCDLogger::getInstance()->log(LOG_INFO, "Starting application");

        // TODO remove this hardcoded file
        std::string dictFileName = "dict-dutch-pt.csv";
        
        PaulosCSVParser cvsParser;

        if (! cvsParser.getDictionary(dictFileName, myDict))
        {
            UCDLogger::getInstance()->log(LOG_ERR, "Not possible to parse dictionary from " + dictFileName);
            return 1;
        }
    
        asio::io_context ioc;
        tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 9002));

        UCDLogger::getInstance()->log(LOG_INFO, "WebSocket server listening on port 9002...");

        for (;;) {
            tcp::socket socket(ioc);
            acceptor.accept(socket);
            std::thread{websocket_worker, std::move(socket)}.detach();
        }
    } catch (const std::exception& e) {
        UCDLogger::getInstance()->log(LOG_CRIT, "Fatal error: " + std::string(e.what()));
    }
    return 0;
}