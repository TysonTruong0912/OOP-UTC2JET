// VePhoThong.cpp
#include "VePhoThong.h"

VePhoThong::VePhoThong(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : VeMayBay(std::move(ma), std::move(ghe), cb, hk) {}


double VePhoThong::tinhGia() const { return chuyenBay->getGiaCoBan() * HE_SO; }


std::string VePhoThong::loaiVe() const { return "Phổ thông"; }
