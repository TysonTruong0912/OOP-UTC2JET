#pragma once
#include "Common.h"
#include "VeMayBay.h"

class VeThuongGia : public VeMayBay {
public:
    VeThuongGia(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk);
    double tinhGia() const override;
    std::string loaiVe() const override;
    bool coPhongCho() const;
private:
    static constexpr double HE_SO = 2.5;
};


// ----- VePhoThong -----
