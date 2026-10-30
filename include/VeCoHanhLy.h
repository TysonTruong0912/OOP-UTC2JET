#pragma once
#include "Common.h"
#include "VeDecorator.h"
#include "HanhLy.h"

class VeCoHanhLy : public VeDecorator {
public:
    VeCoHanhLy(std::unique_ptr<VeMayBay> v, HanhLy hl);
    double tinhGia() const override;
    std::string moTa() const override;
private:
    HanhLy hanhLy;
};


// ----- SuatAn -----
