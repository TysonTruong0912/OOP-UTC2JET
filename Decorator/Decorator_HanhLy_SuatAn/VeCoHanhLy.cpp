// VeCoHanhLy.cpp
#include "VeCoHanhLy.h"

VeCoHanhLy::VeCoHanhLy(std::unique_ptr<VeMayBay> v, HanhLy hl)
    : VeDecorator(std::move(v)), hanhLy(hl) {}


double VeCoHanhLy::tinhGia() const { return ve->tinhGia() + hanhLy.tinhPhi(); }


std::string VeCoHanhLy::moTa() const { return ve->moTa() + " + " + hanhLy.moTa(); }
