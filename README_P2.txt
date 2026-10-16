================================================================================
  MODULE P2 — QUẢN LÝ CHUYẾN BAY
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

Test case module P2 — Quản lý chuyến bay
Class: SanBay, MayBay, HangHangKhong, ChuyenBay, QuanLyChuyenBay; enum HangGhe
TC01 – Thêm / xem / xóa dữ liệu + khóa định danh
Mô tả: Thêm sân bay, máy bay, chuyến bay; thử mã trùng.
Kỳ vọng: Thêm thành công; mã trùng bị từ chối có thông báo.
Kết quả: ☐ Pass  ☐ Fail
Ghi chú: ................................
TC02 – Nạp CSV và dữ liệu sai định dạng
Mô tả: Chạy app nạp data/sanbay, maybay, hang, chuyenbay; file sai dòng (nếu test).
Kỳ vọng: Nạp OK; lỗi định dạng / tham chiếu báo rõ file + dòng.
Kết quả: ☐ Pass  ☐ Fail
Ghi chú: ................................
TC03 – Tìm chuyến + kiểm tra giờ bay
Mô tả: Tìm chuyến theo điểm đi–đến; thêm CB giờ đến ≤ giờ đi.
Kỳ vọng: Ra danh sách / sắp xếp theo giờ; giờ không hợp lệ bị từ chối.
Kết quả: ☐ Pass  ☐ Fail
Ghi chú: ................................
TC04 – Dữ liệu mẫu VNA / VJ / QH / VU
Mô tả: Kiểm tra hang.csv / máy bay / chuyến bay mẫu.
Kỳ vọng: Có đủ 4 hãng VNA, VJ, QH, VU.
Kết quả: ☐ Pass  ☐ Fail
Ghi chú: ................................
TC05 – Trùng lịch cùng máy bay
Mô tả: Thêm 2 chuyến cùng số hiệu máy bay, khung giờ giao nhau.
Kỳ vọng: Từ chối + thông báo trùng lịch máy bay.
Kết quả: ☐ Pass  ☐ Fail
Ghi chú: ................................
TC06 – Biên dịch và tích hợp
Mô tả: cd DT04_DatVe
  dir
  g++ -std=c++17 -O1 -Iinclude src/AirlineSystem.cpp -o datve.exe -lws2_32
  ./datve.exe
Kỳ vọng: Build được; vào menu chính; nạp data/.
Kết quả: ☐ Pass  ☐ Fail
Ghi chú: ................................
Kết luận: ☐ Đủ điều kiện Done  ☐ Còn lỗi (ghi TC fail)

Build hệ thống đầy đủ:
  cd DT04_DatVe
  dir
  g++ -std=c++17 -O1 -Iinclude src/AirlineSystem.cpp -o datve.exe -lws2_32
  ./datve.exe
Data liên quan: data/sanbay.csv, maybay.csv, hang.csv, chuyenbay.csv
================================================================================
