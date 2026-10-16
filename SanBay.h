#pragma once
// SanBay.h — Module P2 Quản lý chuyến bay (khai báo)
#include "Enums.h"
#include <string>
#include <vector>
#include <memory>
#include <chrono>

class SanBay {
public:
    SanBay(std::string ma, std::string ten, std::string thanhPho);
    std::string getMaSB() const;
    std::string getTen() const;
    std::string getThanhPho() const;
private:
    std::string maSB;
    std::string ten;
    std::string thanhPho;
};
