#pragma once
#include "Common.h"
#include "Ghe.h"
#include "MayBay.h"

class SoDoGhe {
public:
    SoDoGhe(const MayBay& mb);
    Ghe* timGhe(std::string soGhe);
    std::vector<Ghe*> layGheTrong(HangGhe h);
    int demGheTrong(HangGhe h) const;
    bool giuGhe(std::string soGhe);
    void huyGiu(std::string soGhe);
    bool datGhe(std::string soGhe);
    void traGhe(std::string soGhe);
    void hienThiSoDo() const;
private:
    std::vector<Ghe> dsGhe;
};


// ----- GiuCho -----
