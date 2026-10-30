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
// San bay "khop" khi chuoi rong (khong loc), hoac trung ma san bay, hoac trung ten thanh pho.
bool khopSanBay(const SanBay* sb, const std::string& s) {
    return s.empty() || (sb && (sb->getMaSB() == s || sb->getThanhPho() == s));
}

bool cungNgay(time_point a, time_point b) {
    std::time_t ta = std::chrono::system_clock::to_time_t(a);
    std::time_t tb = std::chrono::system_clock::to_time_t(b);
    std::tm x{}, y{};
#ifdef _WIN32
    localtime_s(&x, &ta);
    localtime_s(&y, &tb);
#else
    localtime_r(&ta, &x);
    localtime_r(&tb, &y);
#endif
    return x.tm_year == y.tm_year && x.tm_yday == y.tm_yday;
}

// time_point{} (moc epoch) nghia la "khong loc theo ngay".
bool khopNgay(const ChuyenBay* cb, time_point ngay) {
    return ngay == time_point{} || cungNgay(cb->getGioDi(), ngay);
}
}

void QuanLyChuyenBay::themChuyenBay(std::unique_ptr<ChuyenBay> cb) {
    if (!cb) throw LoiNghiepVu("Chuyến bay không hợp lệ");
    if (timChuyenBay(cb->getMaCB()))
        throw LoiNghiepVu("Trùng mã chuyến bay " + cb->getMaCB());
    // Cùng số hiệu máy bay không được trùng/giao khung giờ bay
    MayBay* mb = cb->getMayBay();
    if (mb) {
        const auto di = cb->getGioDi();
        const auto den = cb->getGioDen();
        if (den <= di)
            throw LoiNghiepVu("Giờ đến phải sau giờ đi");
        for (const auto& x : dsChuyenBay) {
            if (!x || !x->getMayBay()) continue;
            if (x->getMayBay()->getSoHieu() != mb->getSoHieu()) continue;
            // Giao nhau: di < x.den && den > x.di
            if (di < x->getGioDen() && den > x->getGioDi()) {
                throw LoiNghiepVu(
                    "Trùng lịch máy bay " + mb->getSoHieu() +
                    " với chuyến " + x->getMaCB() +
                    " (cùng tàu bay, khung giờ giao nhau)");
            }
        }
    }
    dsChuyenBay.push_back(std::move(cb));
}

// Khong cho xoa chuyen bay da co ve / dang giu cho (tranh con tro treo o ve va giu cho).
void QuanLyChuyenBay::xoaChuyenBay(std::string maCB) {
    auto it = std::find_if(dsChuyenBay.begin(), dsChuyenBay.end(),
                           [&](const std::unique_ptr<ChuyenBay>& c) { return c->getMaCB() == maCB; });
    if (it == dsChuyenBay.end()) throw LoiChuyenBayKhongTonTai("Khong co chuyen bay " + maCB);
    ChuyenBay* cb = it->get();
    cb->donDepGiuCho();
    int conTrong = 0;
    for (HangGhe h : {HangGhe::THUONG_GIA, HangGhe::PHO_THONG, HangGhe::TIET_KIEM})
        conTrong += cb->getSoDoGhe().demGheTrong(h);
    if (conTrong != cb->getMayBay()->tongSoGhe())
        throw LoiNghiepVu("Chuyến bay " + maCB + " da co ve hoac dang giu cho, khong the xoa");
    dsChuyenBay.erase(it);
}

void QuanLyChuyenBay::themSanBay(std::unique_ptr<SanBay> sb) {
    if (!sb) throw LoiNghiepVu("San bay khong hop le");
    if (timSanBay(sb->getMaSB())) throw LoiNghiepVu("Trung ma san bay " + sb->getMaSB());
    dsSanBay.push_back(std::move(sb));
}

void QuanLyChuyenBay::themMayBay(std::unique_ptr<MayBay> mb) {
    if (!mb) throw LoiNghiepVu("May bay khong hop le");
    if (timMayBay(mb->getSoHieu())) throw LoiNghiepVu("Trung so hieu may bay " + mb->getSoHieu());
    dsMayBay.push_back(std::move(mb));
}

void QuanLyChuyenBay::themHang(std::unique_ptr<HangHangKhong> h) {
    if (!h) throw LoiNghiepVu("Hang hang khong khong hop le");
    for (const auto& x : dsHang)
        if (x->getMaHang() == h->getMaHang()) throw LoiNghiepVu("Trung ma hang " + h->getMaHang());
    dsHang.push_back(std::move(h));
}

ChuyenBay* QuanLyChuyenBay::timChuyenBay(std::string maCB) const {
    for (const auto& cb : dsChuyenBay) if (cb->getMaCB() == maCB) return cb.get();
    return nullptr;
}

SanBay* QuanLyChuyenBay::timSanBay(std::string maSB) const {
    for (const auto& sb : dsSanBay) if (sb->getMaSB() == maSB) return sb.get();
    return nullptr;
}

MayBay* QuanLyChuyenBay::timMayBay(std::string soHieu) const {
    for (const auto& mb : dsMayBay) if (mb->getSoHieu() == soHieu) return mb.get();
    return nullptr;
}

// di / den: ma san bay hoac thanh pho ("" = bat ky). ngay: time_point{} = bat ky ngay.
std::vector<ChuyenBay*> QuanLyChuyenBay::timChuyenBay(std::string di, std::string den, time_point ngay) const {
    std::vector<ChuyenBay*> kq;
    for (const auto& cb : dsChuyenBay)
        if (khopSanBay(cb->getSanBayDi(), di) && khopSanBay(cb->getSanBayDen(), den) && khopNgay(cb.get(), ngay))
            kq.push_back(cb.get());
    return kq;
}

// Tim hanh trinh noi chuyen (tu 2 den toiDaChang chang): chang sau khoi hanh sau khi chang truoc
// den it nhat choToiThieu, khong di lai san bay da qua. Ket qua sap theo gio den cuoi tang dan.
std::vector<std::vector<ChuyenBay*> > QuanLyChuyenBay::timNoiChuyen(std::string di, std::string den, time_point ngay,
                                                                      std::chrono::minutes choToiThieu,
                                                                      int toiDaChang) const {
    std::vector<std::vector<ChuyenBay*> > kq;
    std::vector<ChuyenBay*> duong;
    std::function<void()> duyet = [&]() {
        ChuyenBay* cuoi = duong.back();
        if (duong.size() >= 2 && khopSanBay(cuoi->getSanBayDen(), den)) kq.push_back(duong);
        if (static_cast<int>(duong.size()) >= toiDaChang) return;
        for (const auto& cb : dsChuyenBay) {
            ChuyenBay* tiep = cb.get();
            if (tiep->getSanBayDi() != cuoi->getSanBayDen()) continue;
            if (tiep->getGioDi() < cuoi->getGioDen() + choToiThieu) continue;
            bool quayLai = false;
            for (const ChuyenBay* c : duong)
                if (c->getSanBayDi() == tiep->getSanBayDen()) { quayLai = true; break; }
            if (quayLai) continue;
            duong.push_back(tiep);
            duyet();
            duong.pop_back();
        }
    };
    for (const auto& cb : dsChuyenBay) {
        if (!khopSanBay(cb->getSanBayDi(), di) || !khopNgay(cb.get(), ngay)) continue;
        duong.assign(1, cb.get());
        duyet();
    }
    std::sort(kq.begin(), kq.end(), [](const std::vector<ChuyenBay*>& a, const std::vector<ChuyenBay*>& b) {
        return a.back()->getGioDen() < b.back()->getGioDen();
    });
    return kq;
}

void QuanLyChuyenBay::sapXepTheoGio(std::vector<ChuyenBay*>& ds) {
    std::sort(ds.begin(), ds.end(), [](const ChuyenBay* a, const ChuyenBay* b) { return *a < *b; });
}

void QuanLyChuyenBay::sapXepTheoGia(std::vector<ChuyenBay*>& ds) {
    std::sort(ds.begin(), ds.end(),
              [](const ChuyenBay* a, const ChuyenBay* b) { return ChuyenBay::soSanhTheoGia(*a, *b); });
}

std::vector<SanBay*> QuanLyChuyenBay::layDsSanBay() const {
    std::vector<SanBay*> kq;
    for (const auto& x : dsSanBay) kq.push_back(x.get());
    return kq;
}
std::vector<MayBay*> QuanLyChuyenBay::layDsMayBay() const {
    std::vector<MayBay*> kq;
    for (const auto& x : dsMayBay) kq.push_back(x.get());
    return kq;
}
std::vector<ChuyenBay*> QuanLyChuyenBay::layDsChuyenBay() const {
    std::vector<ChuyenBay*> kq;
    for (const auto& x : dsChuyenBay) kq.push_back(x.get());
    return kq;
}
std::vector<HangHangKhong*> QuanLyChuyenBay::layDsHang() const {
    std::vector<HangHangKhong*> kq;
    for (const auto& x : dsHang) kq.push_back(x.get());
    return kq;
}
