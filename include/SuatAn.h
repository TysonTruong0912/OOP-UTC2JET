#pragma once
#include "Common.h"
#include "DichVuGiaTang.h"

class SuatAn : public DichVuGiaTang {
public:
    SuatAn(std::string mon, double gia);
    double tinhPhi() const override;
    std::string moTa() const override;
private:
    std::string monAn;
    double gia{};
};


// ----- VeCoSuatAn -----
