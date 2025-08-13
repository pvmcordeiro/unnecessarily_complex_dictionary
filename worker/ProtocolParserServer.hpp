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
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "../commons/UCDProtocol.hpp"
#include "../commons/UCDLogger.hpp"
#include "DictManager.hpp"
#include "PaulosCSVParser.hpp"

/**
 * The target for this class was study callbacks (std::function and lambda),
 * inheritage and the decorator design patter, that end-up not being 
 * implemented here.
 * 
 * The main idea is to slice the protocol with the fields, each message 
 * has a set of fields and when them all matches you have an unique 
 * action that the system must do. In this implementation all 'roots'
 * of the message to be parsed are in a vector, that must be iterate 
 * to get all roots. So, the root points to the next slice that must 
 * match its field with the messages' field, and so on, until it reaches
 * the last slice that contains the callback that executes the action.
 */
#ifdef MORE_COMPLEXITY_PLEASE
class UCDProtocolSlice {
    private:
        std::shared_ptr<UCDProtocolSlice> nextSlice;
        std::function<bool(const UCDPackage&)> comparingFieldsCB = nullptr;
        std::function<UCDPackage(const unsigned int& size, const std::vector<char>& payload)> callBack = nullptr;        
    public:
        UCDProtocolSlice(std::function<bool(const UCDPackage&)> comparingFieldsCallBack) : comparingFieldsCB(comparingFieldsCallBack)  {}
        virtual ~UCDProtocolSlice() = default;

        void assignCallback(std::function<UCDPackage(const unsigned int& size, const std::vector<char>& payload)> cb)
        {
            callBack = cb;
        }

        UCDPackage execCallback(const UCDPackage& pkg)
        {
            UCD_LOGGER(LOG_DEBUG, "Executing call back");
            return callBack(pkg.payloadSize, pkg.payload);            
        }

        bool execCallbackInTree(const UCDPackage& pkg, UCDPackage& resp)
        {
            if (comparingFieldsCB(pkg))
            {
                // If there is a next slice, continue the chain
                if (nextSlice)
                {
                    UCD_LOGGER(LOG_DEBUG, "Going to the next protocol slice");
                    nextSlice->getNextSlice(pkg, resp);
                }

                if (callBack)
                {
                    UCD_LOGGER(LOG_DEBUG, "Found call back");
                    resp = execCallback(pkg);
                    return true;
                } 
            }
            return false;
        }

        std::shared_ptr<UCDProtocolSlice> getNextSlice(const UCDPackage& pkg, UCDPackage& resp)
        {
            std::shared_ptr<UCDProtocolSlice> result = nullptr;
            if (comparingFieldsCB(pkg))
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
            auto myComparison = [format](const UCDPackage &pkg) -> bool {
                UCD_LOGGER(LOG_DEBUG, "Entered payload format slice, value:");
                return (pkg.format == format);
            };
            auto slice =std::make_shared<UCDProtocolSlice>( myComparison);
            nextSlice = slice;
            return slice;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(UCDProtocol::Command command)
        {
            auto myComparison = [command](const UCDPackage &pkg) -> bool {
                UCD_LOGGER(LOG_DEBUG, "Entered command slice");
                return (pkg.command == command);
            };
            auto slice =std::make_shared<UCDProtocolSlice>(myComparison);
            nextSlice = slice;
            return slice;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(UCDProtocol::Response rsp)
        {
            auto myComparison = [rsp](const UCDPackage &pkg) -> bool {
                UCD_LOGGER(LOG_DEBUG, "Entered response slice");
                return (pkg.response == rsp);
            };
            auto slice =std::make_shared<UCDProtocolSlice>(myComparison);
            nextSlice = slice;
            return slice;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(UCDProtocol::Version version)
        {
            auto myComparison = [version](const UCDPackage &pkg) -> bool {
                UCD_LOGGER(LOG_DEBUG, "Entered version slice");
                return (pkg.version == version);
            };
            auto slice =std::make_shared<UCDProtocolSlice>(myComparison);
            nextSlice = slice;
            return slice;
        }
};
#endif

class ProtocolParserServer {
    private:
#ifdef MORE_COMPLEXITY_PLEASE
    std::vector<std::shared_ptr<UCDProtocolSlice>> rootProtocolSlices;
#endif
    public:
    ProtocolParserServer();
    ~ProtocolParserServer();
    UCDPackage processMsg(const UCDPackage&);
};