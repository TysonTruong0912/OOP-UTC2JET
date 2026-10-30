#pragma once
#include "Common.h"
#include "VeMayBay.h"

class VeDecorator : public VeMayBay {
public:
    VeDecorator(std::unique_ptr<VeMayBay> v);
    const VeMayBay* veGoc() const override;
    std::string loaiVe() const override;
    double tinhGia() const override = 0;
protected:
    std::unique_ptr<VeMayBay> ve;
};


// ----- DichVuGiaTang -----
