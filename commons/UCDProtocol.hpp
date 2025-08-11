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
        FILE
    };

    enum class Command {
        SEARCH,
        ADD,
        LOAD_DICT_FROM_FILE,
        LOAD_DICT_FROM_URL,
        LOAD_DICT_FROM_PAULO_CSV,
        DROP_DB,
        RESPONSE = 99
    };

    enum class LoadOptions {
        ADD_MISSING,
        OVERRIDE,
        DROP_DB_BEFORE_LOAD
    };

    enum class Response {
        SUCCESS,
        MULTIPLE_MATCHES,
        WORD_NOT_FOUND,
        FAIL,
        VOID
    };

};

// class CmdFact : public UCDProtocol {

// };

class UCDPackage {
    public:
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