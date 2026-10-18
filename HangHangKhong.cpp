// HangHangKhong.cpp — P2_ChuyenBay
#include "HangHangKhong.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

HangHangKhong::HangHangKhong(std::string ma, std::string ten)
    : maHang(std::move(ma)), ten(std::move(ten)) {}

std::string HangHangKhong::getMaHang() const { return maHang; }

std::string HangHangKhong::getTen() const { return ten; }

void HangHangKhong::themMayBay(MayBay* mb) { if (mb) dsMayBay.push_back(mb); }

void HangHangKhong::themChuyenBay(ChuyenBay* cb) { if (cb) dsChuyenBay.push_back(cb); }

int HangHangKhong::soChuyenBay() const { return static_cast<int>(dsChuyenBay.size()); }