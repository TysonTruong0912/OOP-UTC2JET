#pragma once
#include "Common.h"
#include "VeDecorator.h"
#include "SuatAn.h"

class VeCoSuatAn : public VeDecorator {
public:
    VeCoSuatAn(std::unique_ptr<VeMayBay> v, SuatAn sa);
    double tinhGia() const override;
    std::string moTa() const override;
private:
    SuatAn suatAn;
};


// ----- VeFactory -----
