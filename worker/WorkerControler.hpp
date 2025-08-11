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