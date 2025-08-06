#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <string>

#include "CsvDict.hpp"


namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

void websocket_worker(tcp::socket socket) {
    try {
        websocket::stream<tcp::socket> ws(std::move(socket));
        ws.accept();

        for (;;) {
            beast::flat_buffer buffer;
            ws.read(buffer);
            std::string msg = beast::buffers_to_string(buffer.data());
            std::cout << "[server] Received: " << msg << std::endl;

            ws.text(ws.got_text());
            ws.write(buffer.data());
        }
    } catch (const beast::system_error& se) {
        std::cerr << "[server] WebSocket error: " << se.what() << std::endl;
    }
}

int main() {
    try {
        asio::io_context ioc;
        tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 9002));

        std::cout << "WebSocket server listening on port 9002..." << std::endl;

        for (;;) {
            tcp::socket socket(ioc);
            acceptor.accept(socket);
            std::thread{websocket_worker, std::move(socket)}.detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
    return 0;
}