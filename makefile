# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g -O0 -Wall
LDFLAGS = -lboost_system -lpthread

# Output binary name
TARGET = ucd
BIN_SERVER = ws_server
BIN_CLIENT = ws_client

SRC_SERVER = ws_server.cpp CsvDict.cpp
SRC_CLIENT = ws_client.cpp

# Source files
SRCS = dict.cpp CsvDict.cpp

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
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up
clean:
	rm -f $(TARGET) $(BIN_SERVER) $(BIN_CLIENT) *.o

