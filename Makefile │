# Makefile for Custom Unix Shell
# Run "make" to build, "make clean" to remove build files.

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC = $(wildcard src/*.cpp)
TARGET = shell
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
.PHONY: clean