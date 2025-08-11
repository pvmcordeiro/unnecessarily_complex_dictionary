#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <functional>

#include "../commons/UCDProtocol.hpp"
#include "PaulosCSVParser.hpp"



class WorkerControler : public UCDProtocol {

    private:
        PaulosCSVParser cvsParser;
        std::unordered_map<std::string, std::string> myDict;

    public:
    WorkerControler() = default;
    ~WorkerControler() = default;
    UCDPackage response;

    bool startConnectionThread();
    UCDPackage getCommand();
    void sendResponse(UCDPackage);
    void websocket_worker(
        boost::asio::ip::tcp::socket socket);
    UCDPackage processReceivedMsg(const boost::beast::flat_buffer&);
     
};