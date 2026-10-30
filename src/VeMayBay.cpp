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
std::string dinhDangGio(time_point tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[32];
    std::strftime(buf, sizeof buf, "%d/%m/%Y %H:%M", &tm);
    return buf;
}
}

VeMayBay::VeMayBay(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : maVe(std::move(ma)), soGhe(std::move(ghe)), chuyenBay(cb), hanhKhach(hk),
      ngayDat(std::chrono::system_clock::now()), daCheckIn(false) {}

VeMayBay::~VeMayBay() = default;

std::string VeMayBay::getMaVe() const { return maVe; }
std::string VeMayBay::getSoGhe() const { return soGhe; }
void VeMayBay::setSoGhe(std::string ghe) { soGhe = std::move(ghe); }
ChuyenBay* VeMayBay::getChuyenBay() const { return chuyenBay; }
HanhKhach* VeMayBay::getHanhKhach() const { return hanhKhach; }
bool VeMayBay::isDaCheckIn() const { return daCheckIn; }
void VeMayBay::danhDauCheckIn() { daCheckIn = true; }

std::string VeMayBay::moTa() const { return maVe + " - " + loaiVe(); }

const VeMayBay* VeMayBay::veGoc() const { return this; }

void VeMayBay::inBoardingPass() const {
    const std::string ten = hanhKhach ? hanhKhach->getHoTen() : "?";
    std::cout << "\n+--------------------------------------------------+\n"
              << "|                  BOARDING PASS                   |\n"
              << "+--------------------------------------------------+\n"
              << " Hanh khach : " << ten << "\n"
              << " Ma ve      : " << maVe << "\n"
              << " Hang ve    : " << loaiVe() << "\n";
    if (chuyenBay) {
        const SanBay* di = chuyenBay->getSanBayDi();
        const SanBay* den = chuyenBay->getSanBayDen();
        std::cout << " Chuyến bay : " << chuyenBay->getMaCB() << "\n"
                  << " Hanh trinh : " << (di ? di->getMaSB() : "?") << " -> " << (den ? den->getMaSB() : "?") << "\n"
                  << " Giờ đi     : " << dinhDangGio(chuyenBay->getGioDi()) << "\n"
                  << " Giờ đến    : " << dinhDangGio(chuyenBay->getGioDen()) << "\n";
    }
    std::cout << " So ghe     : " << soGhe << "\n";
    if (const VeThuongGia* tg = dynamic_cast<const VeThuongGia*>(veGoc()))
        std::cout << " Phong cho  : " << (tg->coPhongCho() ? "Phong cho thuong gia" : "Khong") << "\n";
    std::cout << " Chi tiet   : " << moTa() << "\n"
              << "+--------------------------------------------------+\n";
}
