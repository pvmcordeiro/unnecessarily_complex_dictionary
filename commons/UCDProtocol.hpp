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

#include <boost/json.hpp>
#include <vector>
#include <memory>
#include <functional>

#define UCD_PROTOCOL_CURRENT_VERSION 1

class UCDProtocol {

public:
    UCDProtocol() {};
    ~UCDProtocol() {};

    const unsigned int version = 1;

    enum class PayloadFormat {
        VOID,
        STRING,
        JSON,
        CBON,
        MESSAGE_PACK,
        FILE,
        NONE
    };

    enum class Command {
        SEARCH,
        ADD,
        LOAD_DICT_FROM_FILE,
        LOAD_DICT_FROM_URL,
        LOAD_DICT_FROM_PAULO_CSV,
        DROP_DB,
        RESPONSE,
        NONE
    };

    enum class LoadOptions {
        ADD_MISSING,
        OVERRIDE,
        DROP_DB_BEFORE_LOAD,
        NONE
    };

    enum class Response {
        SUCCESS,
        MULTIPLE_MATCHES,
        WORD_NOT_FOUND,
        FAIL,
        VOID,
        NONE
    };

};

class UCDPackage {
    public:
    UCDPackage() = default;
    UCDPackage(unsigned int, UCDProtocol::Command, UCDProtocol::PayloadFormat, UCDProtocol::Response);
    ~UCDPackage() = default;

    unsigned int version;
    UCDProtocol::Command command;
    UCDProtocol::Response response;
    UCDProtocol::PayloadFormat format;
    unsigned int payloadSize;
    std::vector<char> payload;

    boost::json::object serializeUCDPackage() {
        boost::json::object obj;
        obj["command"] = static_cast<int>(command);
        obj["response"] = static_cast<int>(response);
        obj["format"] = static_cast<int>(format);
        obj["payloadSize"] = payloadSize;
        obj["payload"] = std::string(payload.begin(), payload.end());
        return obj;
    }

    void deserializeUCDPackage(const boost::json::value& jv) {
        boost::json::object obj = jv.as_object();

        command = static_cast<UCDProtocol::Command>(obj["command"].as_int64());
        response = static_cast<UCDProtocol::Response>(obj["response"].as_int64());
        format = static_cast<UCDProtocol::PayloadFormat>(obj["format"].as_int64());
        payloadSize = obj["payloadSize"].as_int64();

        std::string payloadStr = obj["payload"].as_string().c_str();
        payload.assign(payloadStr.begin(), payloadStr.end());
    }
};

class UCDProtocolSlice {
    private:
        UCDProtocol::Command command = UCDProtocol::Command::NONE;
        UCDProtocol::PayloadFormat payloadFormat = UCDProtocol::PayloadFormat::NONE;
        UCDProtocol::Response response = UCDProtocol::Response::NONE;
        unsigned int version = 0;        

        std::shared_ptr<UCDProtocolSlice> nextSlice;
        std::function<UCDPackage(const unsigned int& size, const std::vector<char>& payload)> callBack = nullptr;        
    public:
        UCDProtocolSlice(const UCDProtocol::Command cmd) : command(cmd) {}
        UCDProtocolSlice(const UCDProtocol::PayloadFormat fmt) : payloadFormat(fmt) {}
        UCDProtocolSlice(const UCDProtocol::Response rsp) : response(rsp) {}
        UCDProtocolSlice(const unsigned int ver) : version(ver) {}

        //~ŨCDProtocolSlice() = default;
        void assignCallback(std::function<UCDPackage(const unsigned int& size, const std::vector<char>& payload)> _callBack)
        {
            callBack = _callBack;
        }

        UCDPackage execCallback(const UCDPackage& pkg)
        {
            return callBack(pkg.payloadSize, pkg.payload);            
        }

        bool execCallbackInTree(const UCDPackage& pkg, UCDPackage& resp)
        {
            if (command == pkg.command || 
                payloadFormat == pkg.format ||
                response == pkg.response ||
                version == pkg.version)
            {
                // If there is a next slice, continue the chain
                if (nextSlice)
                {
                    nextSlice->getNextSlice(pkg, resp);
                }

                if (callBack)
                {
                    resp = execCallback(pkg);
                    return true;
                } 
            }
            return false;
        }

        std::shared_ptr<UCDProtocolSlice> getNextSlice(const UCDPackage& pkg, UCDPackage& resp)
        {
            std::shared_ptr<UCDProtocolSlice> result = nullptr;
            if (command == pkg.command || 
                payloadFormat == pkg.format ||
                response == pkg.response ||
                version == pkg.version)
            {
                // If there is a next slice, continue the chain
                if (nextSlice)
                {
                    result = nextSlice->getNextSlice(pkg, resp);
                }

                if (callBack)
                {
                    resp = execCallback(pkg);
                } 
            }
            return result;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(UCDProtocol::PayloadFormat format)
        {
            std::shared_ptr<UCDProtocolSlice> slice = std::make_shared<UCDProtocolSlice>(format);
            nextSlice = slice;
            return slice;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(UCDProtocol::Command command)
        {
            std::shared_ptr<UCDProtocolSlice> slice = std::make_shared<UCDProtocolSlice>(command);
            nextSlice = slice;
            return slice;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(UCDProtocol::Response rsp)
        {
            std::shared_ptr<UCDProtocolSlice> slice = std::make_shared<UCDProtocolSlice>(rsp);
            nextSlice = slice;
            return slice;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(unsigned int version)
        {
            std::shared_ptr<UCDProtocolSlice> slice = std::make_shared<UCDProtocolSlice>(version);
            nextSlice = slice;
            return slice;
        }
};

// class UCDProtocolParser {
//     private:
//     public:
//     UCDProtocolParser
// };