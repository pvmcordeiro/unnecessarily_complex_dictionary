# MIT License# 

# Copyright (c) 2025 Paulo Cordeiro# 

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:# 

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# To compile this code with more complexities use: 
# make EXTRA_FLAGS="-DMORE_COMPLEXITY_PLEASE" 
# This will complie a protocol parser for the server, which is more complex
# than the current configuration 

# Compiler and flags
EXTRA_FLAGS ?=

CXX = g++
CXXFLAGS = -std=c++17 -g -O0 -Wall $(EXTRA_FLAGS)
LDFLAGS = -lboost_system -lpthread -lboost_json


# Output binary name
TARGET = ucd
BIN_SERVER = ws_server
BIN_CLIENT = ws_client

SRC_SERVER = ws_server.cpp worker/PaulosCSVParser.cpp worker/DictManager.cpp worker/WorkerControler.cpp worker/ProtocolParserServer.cpp
SRC_CLIENT = ws_client.cpp

# Source files
SRCS = dict.cpp worker/PaulosCSVParser.cpp worker/DictManager.cpp

# Object files (optional if you want to split compilation)
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean install test

# Default rule
all: $(TARGET) $(BIN_SERVER) $(BIN_CLIENT)

$(BIN_SERVER): $(SRC_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN_CLIENT): $(SRC_CLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

# Clean up
clean:
	rm -f $(TARGET) $(BIN_SERVER) $(BIN_CLIENT) *.o

