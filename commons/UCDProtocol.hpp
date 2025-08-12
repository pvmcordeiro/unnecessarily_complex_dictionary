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

/**
 *  This class is the parent for all other protocol fields.
 *  It seems a good idea in the beginning, but it became more 
 *  complicated than foreseen. If someone has a better idea 
 *  how to implement this inheritance, I'm open to suggestions.
 */
class UCDProtocolField {
    public:
    virtual ~UCDProtocolField() = default;
};

class UCDProtocol {

public:
    UCDProtocol() {};
    ~UCDProtocol() {};

    /**
     * The override of the operator is a way to make
     */
    class Version : public UCDProtocolField {
        public:
        u_int32_t version;
        Version(u_int32_t v = UCD_PROTOCOL_CURRENT_VERSION) : version(v) {}
        operator u_int32_t() const {return version;}
    };

    class PayloadFormat : public UCDProtocolField {
        public:
        static constexpr const u_int8_t VOID = 0;
        static constexpr const u_int8_t STRING = 1;
        static constexpr const u_int8_t JSON = 2; 
        static constexpr const u_int8_t CBON = 3;
        static constexpr const u_int8_t MESSAGE_PACK = 4;
        static constexpr const u_int8_t FILE = 5;
        static constexpr const u_int8_t NONE = 99;
        u_int8_t value;
        PayloadFormat(u_int8_t v = NONE) : value(v) {}
        operator u_int8_t() const { return value; }
   };

    class Command : public UCDProtocolField {
        public:
        static constexpr const u_int8_t SEARCH = 0;
        static constexpr const u_int8_t ADD_WORD = 1;
        static constexpr const u_int8_t LOAD_DICT_FROM_FILE = 2;
        static constexpr const u_int8_t LOAD_DICT_FROM_URL = 3;
        static constexpr const u_int8_t LOAD_DICT_FROM_PAULO_CSV = 4;
        static constexpr const u_int8_t DROP_DB = 5;
        static constexpr const u_int8_t RESPONSE = 6;
        static constexpr const u_int8_t NONE = 99;
        u_int8_t value;
        Command(u_int8_t v = NONE) : value(v) {}
        operator u_int8_t() const { return value; }
    };

    class LoadOptions : public UCDProtocolField {
        public:
        static constexpr const u_int8_t ADD_MISSING = 0;
        static constexpr const u_int8_t OVERRIDE = 1;
        static constexpr const u_int8_t DROP_DB_BEFORE_LOAD = 2;
        static constexpr const u_int8_t NONE = 99;
        u_int8_t value;
        LoadOptions(u_int8_t v = NONE) : value(v) {}
        operator u_int8_t() const { return value; }
    };

    class Response : public UCDProtocolField {
        public:
        static constexpr const u_int8_t SUCCESS = 0;
        static constexpr const u_int8_t MULTIPLE_MATCHES = 1;
        static constexpr const u_int8_t WORD_NOT_FOUND = 2;
        static constexpr const u_int8_t FAIL = 3;
        static constexpr const u_int8_t VOID = 4;
        static constexpr const u_int8_t NONE = 99;
        u_int8_t value;
        Response(u_int8_t v = NONE) : value(v) {}
        operator u_int8_t() const { return value; }
    };

};

class UCDPackage {
    public:
    UCDPackage(){};
    ~UCDPackage(){};

    UCDProtocol::Version version;
    UCDProtocol::Command command;
    UCDProtocol::Response response;
    UCDProtocol::PayloadFormat format;
    u_int32_t payloadSize;
    std::vector<char> payload;

    boost::json::object serializeUCDPackage() {
        boost::json::object obj;
        obj["command"] = static_cast<u_int8_t>(command);
        obj["response"] = static_cast<u_int8_t>(response);
        obj["format"] = static_cast<u_int8_t>(format);
        obj["version"] = static_cast<u_int32_t>(version);
        obj["payloadSize"] = payloadSize;
        obj["payload"] = std::string(payload.begin(), payload.end());
        return obj;
    }

    void deserializeUCDPackage(const boost::json::value& jv) {
        boost::json::object obj = jv.as_object();

        command = UCDProtocol::Command(static_cast<u_int8_t>(obj["command"].as_int64()));
        response = static_cast<UCDProtocol::Response>(obj["response"].as_int64());
        format = static_cast<UCDProtocol::PayloadFormat>(obj["format"].as_int64());
        version = static_cast<UCDProtocol::Version>(obj["version"].as_int64());
        payloadSize = obj["payloadSize"].as_int64();
        std::string payloadStr = obj["payload"].as_string().c_str();
        payload.assign(payloadStr.begin(), payloadStr.end());
    }
};


// class UCDProtocolParser {
//     private:
//     public:
//     UCDProtocolParser
// };
