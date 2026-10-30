#pragma once
#include "Common.h"

class Ghe {
public:
    Ghe(std::string so, HangGhe h);
    std::string getSoGhe() const;
    HangGhe getHang() const;
    TrangThaiGhe getTrangThai() const;
    void setTrangThai(TrangThaiGhe tt);
private:
    std::string soGhe;
    HangGhe hang{};
    TrangThaiGhe trangThai{};
};


// ----- SoDoGhe -----
