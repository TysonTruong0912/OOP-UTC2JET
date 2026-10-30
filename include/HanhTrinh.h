#pragma once
#include "Common.h"
#include "ChuyenBay.h"

class HanhTrinh {
public:
    void themChang(ChuyenBay* cb);
    void setHanhKhach(HanhKhach* hk);
    void setHang(HangGhe h);
    void setHanhLy(double kg);
    void setSuatAn(std::string mon, double gia);
    HanhKhach* getHanhKhach() const;
    HangGhe getHang() const;
    const std::vector<ChuyenBay*>& getDsChang() const;
    double getSoKgHanhLy() const;
    std::string getMonAn() const;
    double getGiaSuatAn() const;
    int soChang() const;
    std::chrono::minutes tongThoiGian() const;
private:
    HanhKhach* hanhKhach{};
    HangGhe hang{};
    std::vector<ChuyenBay*> dsChang;
    double soKgHanhLy{};
    std::string monAn;
    double giaSuatAn{};
};


// ----- VeMayBay -----
