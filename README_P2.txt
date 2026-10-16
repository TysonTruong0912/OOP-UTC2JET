================================================================================
  MODULE P2 — QUẢN LÝ CHUYẾN BAY (file để up Jira)
================================================================================
Người phụ trách: phần SanBay / MayBay / HangHangKhong / ChuyenBay / QuanLyChuyenBay

File khai báo (.h):
  Enums.h              — enum HangGhe, TrangThaiGhe
  SanBay.h
  MayBay.h
  HangHangKhong.h
  ChuyenBay.h
  QuanLyChuyenBay.h

File cài đặt (.cpp):
  SanBay.cpp
  MayBay.cpp
  HangHangKhong.cpp
  ChuyenBay.cpp
  QuanLyChuyenBay.cpp  — có kiểm tra trùng lịch máy bay khi themChuyenBay

Up Jira:
  - Attach từng file .h / .cpp vào ticket P2
  - Hoặc zip cả folder P2_QuanLyChuyenBay_Code

Build hệ thống đầy đủ (không build riêng từng file này):
  cd DT04_DatVe
  g++ -std=c++17 -O1 -Iinclude src/AirlineSystem.cpp -o datve.exe -lws2_32

Data liên quan: data/sanbay.csv, maybay.csv, hang.csv, chuyenbay.csv
================================================================================
