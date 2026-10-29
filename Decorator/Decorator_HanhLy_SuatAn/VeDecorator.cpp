// VeDecorator.cpp
#include "VeDecorator.h"

VeDecorator::VeDecorator(std::unique_ptr<VeMayBay> v)
    : VeMayBay(kiemTraVe(v).getMaVe(), v->getSoGhe(), v->getChuyenBay(), v->getHanhKhach()),
      ve(std::move(v)) {
    if (ve->isDaCheckIn()) danhDauCheckIn();
}


const VeMayBay* VeDecorator::veGoc() const { return ve->veGoc(); }


std::string VeDecorator::loaiVe() const { return ve->loaiVe(); }
