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
const std::chrono::seconds THOI_GIAN_GIU_CHO{300};   // giu cho tam thoi 5 phut
const std::chrono::hours MO_CHECK_IN{24};            // mo check-in truoc gio bay 24 gio
const std::chrono::hours DONG_CHECK_IN{1};           // dong check-in truoc gio bay 1 gio
const double DON_GIA_HANH_LY = 50000.0;              // phi hanh ly moi kg

// Ma ve = "VE" + so thu tu lon nhat dang co + 1 (dung duoc ca sau khi nap ve tu file).
std::string taoMaVe(const std::vector<std::unique_ptr<VeMayBay> >& ds) {
    long lonNhat = 0;
    for (const auto& v : ds) {
        const std::string ma = v->getMaVe();
        if (ma.size() > 2 && ma.compare(0, 2, "VE") == 0) {
            try { lonNhat = std::max(lonNhat, std::stol(ma.substr(2))); } catch (...) {}
        }
    }
    char buf[24];
    std::snprintf(buf, sizeof buf, "VE%05ld", lonNhat + 1);
    return buf;
}
}

QuanLyDatVe::QuanLyDatVe(QuanLyChuyenBay& ql) : qlChuyenBay(ql) {}

GiuCho* QuanLyDatVe::giuCho(std::string maCB, std::string soGhe, HanhKhach* hk) {
    if (!hk) throw LoiNghiepVu("Thieu thong tin hanh khach");
    ChuyenBay* cb = qlChuyenBay.timChuyenBay(maCB);
    if (!cb) throw LoiChuyenBayKhongTonTai("Khong co chuyen bay " + maCB);
    return cb->giuGhe(soGhe, hk, THOI_GIAN_GIU_CHO);
}

VeMayBay* QuanLyDatVe::xacNhanThanhToan(GiuCho* gc, HangGhe h, DaiLy* dl) {
    if (!gc) throw LoiGiuChoHetHan("Khong co phien giu cho");
    ChuyenBay* cb = gc->getChuyenBay();
    const std::string soGhe = gc->getSoGhe();
    HanhKhach* hk = gc->getHanhKhach();
    if (!gc->conHieuLuc()) {
        cb->donDepGiuCho();
        throw LoiGiuChoHetHan("Giữ chỗ ghe " + soGhe + " da het han, vui long dat lai");
    }
    Ghe* ghe = cb->getSoDoGhe().timGhe(soGhe);
    if (!ghe || ghe->getTrangThai() != TrangThaiGhe::GIU_CHO)
        throw LoiGiuChoHetHan("Phien giu cho ghe " + soGhe + " khong con hieu luc");
    if (ghe->getHang() != h) {
        cb->huyGiu(soGhe);
        throw LoiNghiepVu("Ghế " + soGhe + " khong thuoc hang ve da chon");
    }
    std::unique_ptr<VeMayBay> ve = VeFactory::taoVe(h, taoMaVe(dsVe), soGhe, cb, hk);
    VeMayBay* raw = ve.get();
    dsVe.push_back(std::move(ve));
    cb->datGhe(soGhe);
    if (dl) dl->congHoaHong(raw->tinhGia());
    return raw;
}

std::vector<VeMayBay*> QuanLyDatVe::datVeTheoHanhTrinh(const HanhTrinh& ht, DaiLy* dl) {
    if (!ht.getHanhKhach() || ht.soChang() == 0) throw LoiNghiepVu("Hanh trinh khong hop le");
    // Buoc 1: giu cho tat ca cac chang; neu mot chang that bai thi tra lai cac ghe da giu.
    std::vector<GiuCho*> dsGc;
    try {
        for (ChuyenBay* cb : ht.getDsChang()) {
            cb->donDepGiuCho();
            std::vector<Ghe*> trong = cb->getSoDoGhe().layGheTrong(ht.getHang());
            if (trong.empty()) throw LoiHetCho("Chuyến bay " + cb->getMaCB() + " het ghe hang nay");
            dsGc.push_back(cb->giuGhe(trong.front()->getSoGhe(), ht.getHanhKhach(), THOI_GIAN_GIU_CHO));
        }
    } catch (...) {
        for (GiuCho* gc : dsGc) gc->getChuyenBay()->huyGiu(gc->getSoGhe());
        throw;
    }
    // Buoc 2: moi chang tao 1 ve bang Factory, co hanh ly / suat an thi boc them Decorator.
    std::vector<VeMayBay*> ketQua;
    for (GiuCho* gc : dsGc) {
        VeMayBay* ve = xacNhanThanhToan(gc, ht.getHang(), nullptr);
        auto it = std::find_if(dsVe.begin(), dsVe.end(),
                               [&](const std::unique_ptr<VeMayBay>& p) { return p.get() == ve; });
        if (ht.getSoKgHanhLy() > 0)
            *it = std::make_unique<VeCoHanhLy>(std::move(*it), HanhLy(ht.getSoKgHanhLy(), DON_GIA_HANH_LY));
        if (!ht.getMonAn().empty())
            *it = std::make_unique<VeCoSuatAn>(std::move(*it), SuatAn(ht.getMonAn(), ht.getGiaSuatAn()));
        ve = it->get();
        if (dl) dl->congHoaHong(ve->tinhGia());
        ketQua.push_back(ve);
    }
    return ketQua;
}

void QuanLyDatVe::huyVe(std::string maVe) {
    auto it = std::find_if(dsVe.begin(), dsVe.end(),
                           [&](const std::unique_ptr<VeMayBay>& v) { return v->getMaVe() == maVe; });
    if (it == dsVe.end()) throw LoiNghiepVu("Khong tim thay ve " + maVe);
    (*it)->getChuyenBay()->traGhe((*it)->getSoGhe());
    dsVe.erase(it);
}

void QuanLyDatVe::checkIn(std::string maVe, std::string soGheMoi) {
    VeMayBay* ve = timVe(maVe);
    if (!ve) throw LoiNghiepVu("Khong tim thay ve " + maVe);
    if (ve->isDaCheckIn()) throw LoiNghiepVu("Ve " + maVe + " da check-in roi");
    ChuyenBay* cb = ve->getChuyenBay();
    const auto bay = std::chrono::system_clock::now();
    if (bay < cb->getGioDi() - MO_CHECK_IN) throw LoiNghiepVu("Chua den gio mo check-in (mo truoc gio bay 24 gio)");
    if (bay > cb->getGioDi() - DONG_CHECK_IN) throw LoiNghiepVu("Da dong check-in (dong truoc gio bay 1 gio)");

    const std::string soGheCu = ve->getSoGhe();
    if (soGheMoi != soGheCu) {
        const VeTietKiem* tk = dynamic_cast<const VeTietKiem*>(ve->veGoc());
        if (tk && !tk->choPhepDoiVe()) throw LoiNghiepVu("Ve tiet kiem khong duoc doi cho ngoi");
        cb->donDepGiuCho();
        SoDoGhe& sd = cb->getSoDoGhe();
        Ghe* gheCu = sd.timGhe(soGheCu);
        Ghe* gheMoi = sd.timGhe(soGheMoi);
        if (!gheMoi) throw LoiNghiepVu("Ghế " + soGheMoi + " khong ton tai");
        if (gheCu && gheMoi->getHang() != gheCu->getHang())
            throw LoiNghiepVu("Chi duoc doi sang ghe cung hang ve");
        if (gheMoi->getTrangThai() != TrangThaiGhe::TRONG || !sd.datGhe(soGheMoi))
            throw LoiHetCho("Ghế " + soGheMoi + " da co nguoi dat hoac dang duoc giu");
        cb->traGhe(soGheCu);
        ve->setSoGhe(soGheMoi);
    }
    ve->danhDauCheckIn();
}

void QuanLyDatVe::inBoardingPass(std::string maVe) const {
    VeMayBay* ve = timVe(maVe);
    if (!ve) throw LoiNghiepVu("Khong tim thay ve " + maVe);
    if (!ve->isDaCheckIn()) throw LoiNghiepVu("Ve " + maVe + " chua check-in, khong the in boarding pass");
    ve->inBoardingPass();
}

VeMayBay* QuanLyDatVe::timVe(std::string maVe) const {
    for (const auto& v : dsVe) if (v->getMaVe() == maVe) return v.get();
    return nullptr;
}

// Phan loai theo ve goc (ke ca ve da boc Decorator) bang dynamic_cast.
std::map<std::string, int> QuanLyDatVe::thongKeLoaiVe() const {
    std::map<std::string, int> kq{{"Thương gia", 0}, {"Phổ thông", 0}, {"Tiết kiệm", 0}};
    for (const auto& v : dsVe) {
        const VeMayBay* goc = v->veGoc();
        if (dynamic_cast<const VeThuongGia*>(goc)) ++kq["Thương gia"];
        else if (dynamic_cast<const VePhoThong*>(goc)) ++kq["Phổ thông"];
        else if (dynamic_cast<const VeTietKiem*>(goc)) ++kq["Tiết kiệm"];
    }
    return kq;
}

double QuanLyDatVe::tinhDoanhThu() const {
    double tong = 0;
    for (const auto& v : dsVe) tong += v->tinhGia();
    return tong;
}

std::vector<VeMayBay*> QuanLyDatVe::layDsVe() const {
    std::vector<VeMayBay*> kq;
    for (const auto& v : dsVe) kq.push_back(v.get());
    return kq;
}
