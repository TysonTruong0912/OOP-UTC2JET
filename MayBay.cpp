// MayBay.cpp — P2_ChuyenBay
#include "MayBay.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

MayBay::MayBay(std::string soHieu, std::string model, int gheTG, int ghePT, int gheTK)
    : soHieu(std::move(soHieu)), model(std::move(model)),
      soGheThuongGia(gheTG), soGhePhoThong(ghePT), soGheTietKiem(gheTK) {}

std::string MayBay::getSoHieu() const { return soHieu; }

int MayBay::getSoGhe(HangGhe h) const {
    switch (h) {
        case HangGhe::THUONG_GIA: return soGheThuongGia;
        case HangGhe::PHO_THONG:  return soGhePhoThong;
        case HangGhe::TIET_KIEM:  return soGheTietKiem;
    }
    return 0;
}

int MayBay::tongSoGhe() const { return soGheThuongGia + soGhePhoThong + soGheTietKiem; }

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