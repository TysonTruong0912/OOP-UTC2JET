// SanBay.cpp — P2_ChuyenBay
#include "SanBay.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

SanBay::SanBay(std::string ma, std::string ten, std::string thanhPho)
    : maSB(std::move(ma)), ten(std::move(ten)), thanhPho(std::move(thanhPho)) {}

std::string SanBay::getMaSB() const { return maSB; }

std::string SanBay::getTen() const { return ten; }

std::string SanBay::getThanhPho() const { return thanhPho; }