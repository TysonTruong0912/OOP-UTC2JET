#include "AirlineSystem.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <stdexcept>
#include <regex>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;
namespace fs = std::filesystem;

HanhKhach::HanhKhach(std::string ma, std::string ten, std::string mk, std::string cccd,
                     std::string sdt, std::string email)
    : NguoiDung(std::move(ma), std::move(ten), std::move(mk)),
      cccd(std::move(cccd)), soDienThoai(std::move(sdt)), email(std::move(email)) {}

std::string HanhKhach::getCCCD() const { return cccd; }
std::string HanhKhach::getSoDienThoai() const { return soDienThoai; }
std::string HanhKhach::getEmail() const { return email; }

void HanhKhach::hienMenu() {
    std::cout << "\n===== MENU HÀNH KHÁCH =====\n"
              << " 1. Tìm chuyến bay\n"
              << " 2. Tìm chuyến nối chuyến\n"
              << " 3. Xem sơ đồ ghế\n"
              << " 4. Đặt vé (giữ chỗ 5 phút)\n"
              << " 5. Đặt vé theo hành trình (nhiều chặng, hành lý, suất ăn)\n"
              << " 6. Check-in chọn chỗ\n"
              << " 7. In boarding pass\n"
              << " 8. Hủy vé\n"
              << " 9. Xuất file WEB HTML (datve.html - báo cáo)\n"
              << "10. *** MỞ WEB ĐẦY ĐỦ (console -> web, 3 vai trò) ***\n"
              << "11. So sánh giá / mua vé qua đại lý\n"
              << " 0. Đăng xuất\n";
}

std::string HanhKhach::vaiTro() const { return "Hanh khach"; }
