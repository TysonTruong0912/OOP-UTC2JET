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

namespace {
std::string ngayHeThongHomNay() {
    using clock = std::chrono::system_clock;
    const std::time_t t = clock::to_time_t(clock::now());
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[16]{};
    std::strftime(buf, sizeof buf, "%Y-%m-%d", &tm);
    return buf;
}
}

DaiLy::DaiLy(std::string ma, std::string ten, std::string mk, double tyLe)
    : NguoiDung(std::move(ma), std::move(ten), std::move(mk)),
      tyLeHoaHong(tyLe), tongHoaHong(0.0),
      hhBanVeHomNay(0.0), hhThiTruongHomNay(0.0), hhHomQua(0.0),
      ngayHoaHong(ngayHeThongHomNay()) {
    dongBoNgayThuc();
}

// Muc HH thi truong theo TUNG NGAY (lich may) — moi dai ly khac nhau;
// ngay moi co the TANG hoac GIAM so voi hom qua (khong phu thuoc sang/toi).
static double uocHoaHongThiTruong(const std::string& maDL, double tyLe, const std::string& ngay) {
    unsigned h = 2166136261u;
    for (unsigned char c : ngay) { h ^= c; h *= 16777619u; }
    for (unsigned char c : maDL) { h ^= c; h *= 16777619u; }
    // Muc co so theo ty le hoa hong
    const double base = 1200000.0 + tyLe * 32000000.0;  // vd 5% ~ 2.8tr / ngay
    // Bien dong ngay: -18% .. +18% (co ngay len, co ngay xuong)
    const double wave = (static_cast<int>(h % 2001) - 1000) / 1000.0;
    const double dayVal = base * (1.0 + 0.18 * wave);
    return dayVal > 0 ? dayVal : 0;
}

void DaiLy::dongBoNgayThuc() {
    const std::string homNay = ngayHeThongHomNay();
    const double marketNow = uocHoaHongThiTruong(getMaND(), tyLeHoaHong, homNay);

    if (ngayHoaHong != homNay) {
        // Sang ngay moi: chot moc hom qua, reset HH ban ve trong ngay, gan muc thi truong ngay moi
        if (ngayHoaHong.empty()) {
            hhHomQua = marketNow; // lan dau: lay moc tham chieu
        } else {
            hhHomQua = hhBanVeHomNay + hhThiTruongHomNay;
            if (hhHomQua <= 0)
                hhHomQua = uocHoaHongThiTruong(getMaND(), tyLeHoaHong, ngayHoaHong);
        }
        hhBanVeHomNay = 0.0;
        ngayHoaHong = homNay;
        hhThiTruongHomNay = marketNow;
        // Cap nhat tong theo muc thi truong ngay moi (phan in vao tong tich luy)
        tongHoaHong += marketNow * 0.25;
    } else {
        // Cung ngay: giu nguyen muc thi truong (khong tu tang vi "toi")
        if (hhThiTruongHomNay <= 0)
            hhThiTruongHomNay = marketNow;
        else
            hhThiTruongHomNay = marketNow; // dong bo lai cong thuc ngay (on dinh trong ngay)
    }
}

void DaiLy::congHoaHong(double giaTriVe) {
    dongBoNgayThuc();
    const double hh = giaTriVe * tyLeHoaHong;
    tongHoaHong += hh;
    hhBanVeHomNay += hh;
}

double DaiLy::getTongHoaHong() const { return tongHoaHong; }
double DaiLy::getHoaHongHomNay() const { return hhBanVeHomNay + hhThiTruongHomNay; }
double DaiLy::getHoaHongHomQua() const { return hhHomQua; }
double DaiLy::getHoaHongBanVeHomNay() const { return hhBanVeHomNay; }
double DaiLy::getHoaHongThiTruongHomNay() const { return hhThiTruongHomNay; }
double DaiLy::getChenhLechVoiHomQua() const { return getHoaHongHomNay() - hhHomQua; }
std::string DaiLy::getNgayHoaHong() const { return ngayHoaHong; }
void DaiLy::setTongHoaHong(double t) { tongHoaHong = t < 0 ? 0 : t; }
void DaiLy::setHoaHongHomQua(double h) { hhHomQua = h < 0 ? 0 : h; }
void DaiLy::setHoaHongHomNay(double h, const std::string& ngay) {
    // Tuong thich file cu: coi toan bo la phan thi truong + ban ve hop nhat
    hhThiTruongHomNay = h < 0 ? 0 : h;
    hhBanVeHomNay = 0;
    ngayHoaHong = ngay.empty() ? ngayHeThongHomNay() : ngay;
}

void DaiLy::hienMenu() {
    std::cout << "\n===== MENU ĐẠI LÝ =====\n"
              << " 1. Tìm chuyến bay\n"
              << " 2. Xem sơ đồ ghế\n"
              << " 3. Đặt vé hộ khách\n"
              << " 4. Xem hoa hồng\n"
              << " 5. Xuất file WEB HTML (datve.html - báo cáo)\n"
              << " 6. Tình trạng ghế/vé từng chuyến (+ xuất file)\n"
              << " 7. Quy định ưu đãi hạng vé & hành lý\n"
              << " 8. *** MỞ WEB ĐẦY ĐỦ (console -> web, 3 vai trò) ***\n"
              << " 0. Đăng xuất\n";
}

std::string DaiLy::vaiTro() const { return "Dai ly"; }
