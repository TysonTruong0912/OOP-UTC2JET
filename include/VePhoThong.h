#pragma once
#include "Common.h"
#include "VeMayBay.h"

class VePhoThong : public VeMayBay {
public:
    VePhoThong(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk);
    double tinhGia() const override;
    std::string loaiVe() const override;
private:
    static constexpr double HE_SO = 1.0;
};


// ----- VeTietKiem -----
