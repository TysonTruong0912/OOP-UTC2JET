#pragma once
#include "Common.h"

class MayBay {
public:
    MayBay(std::string soHieu, std::string model, int gheTG, int ghePT, int gheTK);
    std::string getSoHieu() const;
    int getSoGhe(HangGhe h) const;
    int tongSoGhe() const;
private:
    std::string soHieu;
    std::string model;
    int soGheThuongGia{};
    int soGhePhoThong{};
    int soGheTietKiem{};
};


// ----- HangHangKhong -----
