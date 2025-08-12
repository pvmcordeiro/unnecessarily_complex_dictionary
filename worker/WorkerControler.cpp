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
#include "WorkerControler.hpp"

#include <iostream>
#include <thread>
#include <string>

#include "../commons/UCDLogger.hpp"
#include "ProtoParserServer.hpp"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

UCDPackage WorkerControler::processReceivedMsg(const boost::beast::flat_buffer& buff)
{
    std::string msg = beast::buffers_to_string(buff.data());
    boost::json::value jv = boost::json::parse(msg);

    ProtocolParserForServer parser;
    UCDPackage pack;
    pack.deserializeUCDPackage(jv);
    UCDPackage response = parser.processMsg(pack);

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

