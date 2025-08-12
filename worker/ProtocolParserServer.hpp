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

class UCDProtocolSlice {
    private:
    std::shared_ptr<UCDProtocolSlice> nextSlice;
    std::function<bool(const UCDPackage&)> comparingFieldsCB = nullptr;
    std::function<UCDPackage(const unsigned int& size, const std::vector<char>& payload)> callBack = nullptr;        
    public:
        UCDProtocolSlice(std::function<bool(const UCDPackage&)> _comparingFieldCB) : comparingFieldsCB(_comparingFieldCB)  {}
        virtual ~UCDProtocolSlice() = default;

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
            if (comparingFieldsCB(pkg))
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
                return (pkg.format == format);
            };
            auto slice =std::make_shared<UCDProtocolSlice>( myComparison);
            nextSlice = slice;
            return slice;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(UCDProtocol::Command command)
        {
            auto myComparison = [command](const UCDPackage &pkg) -> bool {
                return (pkg.command == command);
            };
            auto slice =std::make_shared<UCDProtocolSlice>(myComparison);
            nextSlice = slice;
            return slice;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(UCDProtocol::Response rsp)
        {
            auto myComparison = [rsp](const UCDPackage &pkg) -> bool {
                return (pkg.response == rsp);
            };
            auto slice =std::make_shared<UCDProtocolSlice>(myComparison);
            nextSlice = slice;
            return slice;
        }

        std::shared_ptr<UCDProtocolSlice> addSlice(UCDProtocol::Version version)
        {
            auto myComparison = [version](const UCDPackage &pkg) -> bool {
                return (pkg.version == version);
            };
            auto slice =std::make_shared<UCDProtocolSlice>(myComparison);
            nextSlice = slice;
            return slice;
        }
};

class ProtocolParserServer {
    private:
    std::vector<std::shared_ptr<UCDProtocolSlice>> rootProtocolSlices;
    public:
    ProtocolParserServer();
    ~ProtocolParserServer();
    UCDPackage processMsg(const UCDPackage&);
};