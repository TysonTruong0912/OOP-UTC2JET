Cấu trúc C++ đã tách (để cô xem core OOP / nộp Jira)

include/
  Common.h, Enums.h, Forward.h
  NguoiDung.h, Admin.h, DaiLy.h, HanhKhach.h, ...
  (mỗi class 1 file .h)
  P1_NguoiDung.h … P5_MenuFileWeb.h  (gom theo 5 phần)
  AirlineSystem.h  (include tất cả)

src/
  NguoiDung.cpp, Admin.cpp, ChuyenBay.cpp, Menu.cpp, ...
  (mỗi class 1 file .cpp — nội dung tách từ bản gốc)
  Helpers.cpp, main.cpp
  AirlineSystem.cpp  — file implementation ĐẦY ĐỦ để BIÊN DỊCH CHẠY (logic không đổi)

Biên dịch (Windows):
  g++ -std=c++17 -O1 -Iinclude src/AirlineSystem.cpp -o datve.exe -lws2_32
  .\datve.exe

Hoặc: make
