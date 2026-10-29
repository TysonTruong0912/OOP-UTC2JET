// VeTietKiem.cpp
#include "VeTietKiem.h"

VeTietKiem::VeTietKiem(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : VeMayBay(std::move(ma), std::move(ghe), cb, hk) {}


double VeTietKiem::tinhGia() const { return chuyenBay->getGiaCoBan() * HE_SO; }


std::string VeTietKiem::loaiVe() const { return "Tiết kiệm"; }


bool VeTietKiem::choPhepDoiVe() const { return false; }
