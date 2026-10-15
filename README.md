# Hệ thống Quản lý Đặt vé Máy bay (BTL OOP — DT04)

**Airline System Management** · C++17 · Lập trình hướng đối tượng

## Giới thiệu

Đồ án mô phỏng hệ thống đặt vé và điều hành chuyến bay:

- **3 vai trò:** Admin, Đại lý, Hành khách  
- **Console** đầy đủ menu nghiệp vụ  
- **Web demo** đồng bộ dữ liệu với console (máy chủ HTTP nhúng trong chương trình)  
- **34 class** OOP: kế thừa, Factory, Decorator, Builder, State ghế  
- **Lưu file CSV/TXT** (không dùng database)

## Tính năng chính

| Vai trò | Chức năng |
|--------|-----------|
| Admin | Quản lý sân bay, máy bay, chuyến bay, thống kê, đại lý, hoa hồng |
| Đại lý | Đặt vé hộ, hoa hồng theo ngày (thị trường + bán vé) |
| Hành khách | Đăng ký, tìm/đặt vé, nối chuyến, so sánh đại lý, suất ăn & hành lý |

Web: đăng nhập theo vai trò, đặt vé, sơ đồ ghế, quản trị — cập nhật theo dữ liệu console/`data/`.

## Cấu trúc thư mục

```
DT04_DatVe/
├── include/           # AirlineSystem.h + module P1–P5
├── src/
│   └── AirlineSystem.cpp   # Cài đặt class + web server + main
├── data/              # CSV + TXT (sân bay, chuyến, user, vé, thực đơn…)
├── BTL_OOP.cpp        # Bản gom 1 file (tùy chọn)
├── Makefile
├── CHAY.txt
├── build_va_chay.bat
├── datve.html         # Trang mở nhanh tới web local
└── README.md
```

## Biên dịch & chạy (Windows)

Cần **g++** (MinGW-w64). Trong thư mục `DT04_DatVe`:

```powershell
g++ -std=c++17 -O1 -Iinclude src/AirlineSystem.cpp -o datve.exe -lws2_32
.\datve.exe
```

Hoặc double-click `build_va_chay.bat`.

**Web:** trong menu console chọn mở web đầy đủ → trình duyệt:

- http://localhost:8080  
- hoặc mở file `datve.html`

### Tài khoản mẫu

| Vai trò | Mã | Mật khẩu |
|---------|-----|----------|
| Admin | AD01 | admin123 |
| Đại lý | DL01 … DL05 | daily123 |
| Hành khách | tự đăng ký | |

## Công nghệ

- C++17, Winsock2 (HTTP server nhúng)  
- UTF-8 console/web  
- Persistence: `data/*.csv`, `data/*.txt`

## Nhóm / Github

Chia 5 phần: P1 Người dùng · P2 Chuyến bay · P3 Vé-ghế · P4 Decorator-nối chuyến · P5 Menu-File-Web.

Tài liệu UML & chia việc nằm folder **DT04_TaiLieu** (không bắt buộc trong repo code).

## Giấy phép

Đồ án học tập — UTC2 / môn OOP.
