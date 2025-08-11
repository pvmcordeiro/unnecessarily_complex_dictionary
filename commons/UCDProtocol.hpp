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