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
const MayBay& kiemTraMayBay(const MayBay* mb) {
    if (!mb) throw LoiNghiepVu("Chuyến bay phai gan voi mot may bay");
    return *mb;
}
}

ChuyenBay::ChuyenBay(std::string ma, SanBay* di, SanBay* den, MayBay* mb, time_point gioDi,
                     time_point gioDen, double giaCoBan)
    : maCB(std::move(ma)), sanBayDi(di), sanBayDen(den), mayBay(mb), soDoGhe(kiemTraMayBay(mb)),
      gioDi(gioDi), gioDen(gioDen), giaCoBan(giaCoBan) {}

std::string ChuyenBay::getMaCB() const { return maCB; }
SanBay* ChuyenBay::getSanBayDi() const { return sanBayDi; }
SanBay* ChuyenBay::getSanBayDen() const { return sanBayDen; }
MayBay* ChuyenBay::getMayBay() const { return mayBay; }
time_point ChuyenBay::getGioDi() const { return gioDi; }
time_point ChuyenBay::getGioDen() const { return gioDen; }
double ChuyenBay::getGiaCoBan() const { return giaCoBan; }
SoDoGhe& ChuyenBay::getSoDoGhe() { return soDoGhe; }

std::chrono::minutes ChuyenBay::thoiGianBay() const {
    return std::chrono::duration_cast<std::chrono::minutes>(gioDen - gioDi);
}

void ChuyenBay::dichGio(std::chrono::seconds delta) {
    gioDi += delta;
    gioDen += delta;
}

void ChuyenBay::datGioBay(time_point gioDiMoi, time_point gioDenMoi) {
    if (gioDenMoi <= gioDiMoi)
        throw LoiNghiepVu("Giờ đến phải sau giờ đi");
    gioDi = gioDiMoi;
    gioDen = gioDenMoi;
}



// Giữ chỗ tam thoi co timeout. Cac phien giu cho (GiuCho) duoc giu den khi chuyen bay bi huy
// de con tro GiuCho* tra ra ngoai khong bao gio bi treo (dangling).
GiuCho* ChuyenBay::giuGhe(std::string soGhe, HanhKhach* hk, std::chrono::seconds timeout) {
    donDepGiuCho();
    if (!soDoGhe.timGhe(soGhe))
        throw LoiHetCho("Ghế " + soGhe + " khong ton tai tren chuyen bay " + maCB);
    if (!soDoGhe.giuGhe(soGhe))
        throw LoiHetCho("Ghế " + soGhe + " da co nguoi dat hoac dang duoc giu");
    dsGiuCho.push_back(std::make_unique<GiuCho>(this, soGhe, hk, timeout));
    return dsGiuCho.back().get();
}

void ChuyenBay::huyGiu(std::string soGhe) { soDoGhe.huyGiu(soGhe); }

// Tra ve trang thai TRONG cho cac ghe co phien giu cho het han (khong co phien nao con hieu luc).
void ChuyenBay::donDepGiuCho() {
    for (const auto& gc : dsGiuCho) {
        if (gc->conHieuLuc()) continue;
        bool conPhienKhacHieuLuc = false;
        for (const auto& khac : dsGiuCho)
            if (khac->getSoGhe() == gc->getSoGhe() && khac->conHieuLuc()) { conPhienKhacHieuLuc = true; break; }
        if (!conPhienKhacHieuLuc) soDoGhe.huyGiu(gc->getSoGhe());
    }
}

void ChuyenBay::datGhe(std::string soGhe) {
    if (!soDoGhe.datGhe(soGhe))
        throw LoiHetCho("Khong the dat ghe " + soGhe + " tren chuyen bay " + maCB);
}

void ChuyenBay::traGhe(std::string soGhe) { soDoGhe.traGhe(soGhe); }

bool ChuyenBay::operator<(const ChuyenBay& o) const { return gioDi < o.gioDi; }

bool ChuyenBay::soSanhTheoGia(const ChuyenBay& a, const ChuyenBay& b) { return a.giaCoBan < b.giaCoBan; }
