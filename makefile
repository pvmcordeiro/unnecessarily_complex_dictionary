# Compiler and flags
CXX = g++
CXXFLAGS = -g -O0 -Wall

# Output binary name
TARGET = ucd

# Source files
SRCS = dict.cpp CsvDict.cpp

# Object files (optional if you want to split compilation)
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up
clean:
	rm -f $(TARGET) *.o

