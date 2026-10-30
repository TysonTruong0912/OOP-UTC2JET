#pragma once
#include "Common.h"
#include "DichVuGiaTang.h"

class HanhLy : public DichVuGiaTang {
public:
    HanhLy(double kg, double donGiaKg);
    double tinhPhi() const override;
    std::string moTa() const override;
private:
    double soKg{};
    double donGiaKg{};
};


// ----- VeCoHanhLy -----
