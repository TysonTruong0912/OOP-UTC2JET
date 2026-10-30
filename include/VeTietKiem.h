#pragma once
#include "Common.h"
#include "VeMayBay.h"

class VeTietKiem : public VeMayBay {
public:
    VeTietKiem(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk);
    double tinhGia() const override;
    std::string loaiVe() const override;
    bool choPhepDoiVe() const;
private:
    static constexpr double HE_SO = 0.8;
};


// ----- VeDecorator -----
