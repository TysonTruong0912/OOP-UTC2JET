#pragma once
#include "Common.h"

class DichVuGiaTang {
public:
    DichVuGiaTang(std::string ma, std::string ten);
    virtual ~DichVuGiaTang();
    virtual double tinhPhi() const = 0;
    virtual std::string moTa() const = 0;
protected:
    std::string maDV;
    std::string ten;
};


// ----- HanhLy -----
