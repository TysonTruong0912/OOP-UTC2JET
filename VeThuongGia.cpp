// VeThuongGia.cpp
#include "VeThuongGia.h"

VeThuongGia::VeThuongGia(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : VeMayBay(std::move(ma), std::move(ghe), cb, hk) {}


double VeThuongGia::tinhGia() const { return chuyenBay->getGiaCoBan() * HE_SO; }


std::string VeThuongGia::loaiVe() const { return "Thương gia"; }


bool VeThuongGia::coPhongCho() const { return true; }
