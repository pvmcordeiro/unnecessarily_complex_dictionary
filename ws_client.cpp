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
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <string>

#include "commons/UCDLogger.hpp"
#include "commons/UCDProtocol.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

int main(int argc, char **argv)
{
    UCD_LOGGER(LOG_INFO, "Started client");
    std::cout << "--- Unncessarily Complex Dictionary --- \n type 'quit' or 'exit' to finish the program" << "\n\n";

    try
    {
        // Check command line arguments.
        if (argc != 3)
        {
            std::cerr << "Usage: ws_client <host> <port>\n";
            return 1;
        }
        std::string host = argv[1];
        std::string port = argv[2];
        std::string word;

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        net::connect(ws.next_layer(), results.begin(), results.end());

        // Perform the WebSocket handshake
        ws.handshake(host, "/");

        beast::flat_buffer buffer;

        while ( true )
        {   
            std::cout << "Enter a dutch word: " ;
            std::cin >> word;
            if (!(word.compare("exit") && word.compare("quit")))
            {
                UCD_LOGGER(LOG_INFO, "Exited app");
                break;
            }

            UCDPackage upk, resp;
            upk.command = UCDProtocol::Command::SEARCH;
            upk.format = UCDProtocol::PayloadFormat::STRING;
            upk.payloadSize = word.size();
            upk.payload.assign(word.begin(), word.end());
            
            
            // Send a message
            boost::json::object obj = upk.serializeUCDPackage();
            std::string jsonStr = boost::json::serialize(obj);
            ws.write(net::buffer(jsonStr));

            // Receive a message
            ws.read(buffer);

            resp.deserializeUCDPackage(boost::json::parse(beast::buffers_to_string(buffer.data())));
            
            //std::cout << beast::make_printable(buffer.data()) << std::endl;
            std::cout << std::string(resp.payload.begin(), resp.payload.end()) << std::endl;
            buffer.consume(buffer.size());
        }

        // Close the WebSocket connection
        ws.close(websocket::close_code::normal);
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        UCD_LOGGER(LOG_CRIT, "Error: " + std::string(e.what()));

        return 1;
    }
    return 0;
}