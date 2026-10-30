#pragma once
#include "Common.h"
#include "HanhTrinh.h"
#include "ChuyenBay.h"

class HanhTrinhBuilder {
public:
    HanhTrinhBuilder();
    HanhTrinhBuilder& themChang(ChuyenBay* cb);
    HanhTrinhBuilder& chonHang(HangGhe h);
    HanhTrinhBuilder& chonHanhKhach(HanhKhach* hk);
    HanhTrinhBuilder& themHanhLy(double kg);
    HanhTrinhBuilder& themSuatAn(std::string mon, double gia);
    HanhTrinh build();
private:
    HanhTrinh hanhTrinh;
};


// ----- QuanLyChuyenBay -----
