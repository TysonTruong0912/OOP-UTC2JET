// SanBay.cpp — Module P2 Quản lý chuyến bay (cài đặt)
// Tách từ DT04_DatVe/src/AirlineSystem.cpp — up Jira phần P2
#include "SanBay.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <utility>

SanBay::SanBay(std::string ma, std::string ten, std::string thanhPho)
    : maSB(std::move(ma)), ten(std::move(ten)), thanhPho(std::move(thanhPho)) {}


std::string SanBay::getMaSB() const { return maSB; }


std::string SanBay::getTen() const { return ten; }


std::string SanBay::getThanhPho() const { return thanhPho; }
