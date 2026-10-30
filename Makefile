CXX = g++
CXXFLAGS = -std=c++17 -O1 -Iinclude
LDFLAGS = -lws2_32
TARGET = datve.exe

all: $(TARGET)
$(TARGET): src/AirlineSystem.cpp include/AirlineSystem.h
	$(CXX) $(CXXFLAGS) src/AirlineSystem.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
.PHONY: all clean
