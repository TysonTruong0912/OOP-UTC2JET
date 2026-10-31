// GiuCho.cpp
#include "GiuCho.h"

GiuCho::GiuCho(ChuyenBay* cb, std::string ghe, HanhKhach* hk, std::chrono::seconds timeout)
    : chuyenBay(cb), soGhe(std::move(ghe)), hanhKhach(hk),
      hetHan(std::chrono::steady_clock::now() + timeout) {}


ChuyenBay* GiuCho::getChuyenBay() const { return chuyenBay; }

std::string GiuCho::getSoGhe() const { return soGhe; }

HanhKhach* GiuCho::getHanhKhach() const { return hanhKhach; }

bool GiuCho::conHieuLuc() const { return std::chrono::steady_clock::now() < hetHan; }
