#pragma once
#include "Common.h"
#include "VeMayBay.h"
#include "ChuyenBay.h"
#include "HanhKhach.h"

class VeFactory {
public:
    static std::unique_ptr<VeMayBay> taoVe(HangGhe h, std::string maVe, std::string soGhe, ChuyenBay* cb, HanhKhach* hk);
};


// ----- HanhTrinhBuilder -----
