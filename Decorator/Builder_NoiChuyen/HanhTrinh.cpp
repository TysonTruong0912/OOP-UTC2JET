// HanhTrinh.cpp
#include "HanhTrinh.h"

void HanhTrinh::themChang(ChuyenBay* cb) { if (cb) dsChang.push_back(cb); }

void HanhTrinh::setHanhKhach(HanhKhach* hk) { hanhKhach = hk; }

void HanhTrinh::setHang(HangGhe h) { hang = h; }

void HanhTrinh::setHanhLy(double kg) { soKgHanhLy = kg; }

void HanhTrinh::setSuatAn(std::string mon, double gia) { monAn = std::move(mon); giaSuatAn = gia; }


HanhKhach* HanhTrinh::getHanhKhach() const { return hanhKhach; }

HangGhe HanhTrinh::getHang() const { return hang; }

const std::vector<ChuyenBay*>& HanhTrinh::getDsChang() const { return dsChang; }

double HanhTrinh::getSoKgHanhLy() const { return soKgHanhLy; }

std::string HanhTrinh::getMonAn() const { return monAn; }

double HanhTrinh::getGiaSuatAn() const { return giaSuatAn; }

int HanhTrinh::soChang() const { return static_cast<int>(dsChang.size()); }


std::chrono::minutes HanhTrinh::tongThoiGian() const {
    if (dsChang.empty()) return std::chrono::minutes(0);
    return std::chrono::duration_cast<std::chrono::minutes>(dsChang.back()->getGioDen() - dsChang.front()->getGioDi());
}
