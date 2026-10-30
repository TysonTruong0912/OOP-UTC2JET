#pragma once
#include "Common.h"
#include "ChuyenBay.h"
#include "HanhKhach.h"

class GiuCho {
public:
    GiuCho(ChuyenBay* cb, std::string ghe, HanhKhach* hk, std::chrono::seconds timeout);
    ChuyenBay* getChuyenBay() const;
    std::string getSoGhe() const;
    HanhKhach* getHanhKhach() const;
    bool conHieuLuc() const;
private:
    ChuyenBay* chuyenBay{};
    std::string soGhe;
    HanhKhach* hanhKhach{};
    std::chrono::steady_clock::time_point hetHan{};
};


// ----- ChuyenBay -----
