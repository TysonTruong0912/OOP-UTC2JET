CXX = g++
CXXFLAGS = -std=c++17 -O1 -Iinclude
ifeq ($(OS),Windows_NT)
  LDFLAGS = -lws2_32
  TARGET = datve.exe
else
  LDFLAGS =
  TARGET = datve
endif

# Biên dịch chạy được: 1 file implementation đầy đủ (logic không đổi)
# Các file src/NguoiDung.cpp, ChuyenBay.cpp, ... là bản TÁCH theo class để nộp/xem (Jira)

all: $(TARGET)
$(TARGET): src/AirlineSystem.cpp include/AirlineSystem.h
	$(CXX) $(CXXFLAGS) src/AirlineSystem.cpp -o $(TARGET) $(LDFLAGS)

# Tùy chọn: liệt kê file đã tách (không dùng để link vì anonymous namespace phụ thuộc)
list-split:
	@echo "=== Headers ===" && ls include/*.h
	@echo "=== Sources tách class ===" && ls src/*.cpp | grep -v AirlineSystem

clean:
	rm -f $(TARGET) src/*.o
.PHONY: all clean list-split
