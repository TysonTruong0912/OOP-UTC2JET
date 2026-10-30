#pragma once
#include "Common.h"
#include "MayBay.h"

class HangHangKhong {
public:
    HangHangKhong(std::string ma, std::string ten);
    std::string getMaHang() const;
    std::string getTen() const;
    void themMayBay(MayBay* mb);
    void themChuyenBay(ChuyenBay* cb);
    int soChuyenBay() const;
private:
    std::string maHang;
    std::string ten;
    std::vector<MayBay*> dsMayBay;
    std::vector<ChuyenBay*> dsChuyenBay;
};


// ----- Ghế -----
