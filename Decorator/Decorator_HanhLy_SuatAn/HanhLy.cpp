// HanhLy.cpp
#include "HanhLy.h"

HanhLy::HanhLy(double kg, double donGiaKg)
    : DichVuGiaTang("HL", "Hanh ly"), soKg(kg), donGiaKg(donGiaKg) {}


double HanhLy::tinhPhi() const { return soKg * donGiaKg; }

std::string HanhLy::moTa() const {
    std::ostringstream os;
    os << std::setprecision(10) << "Hanh ly " << soKg << "kg (" << donGiaKg << "/kg)";
    return os.str();
}

VeCoHanhLy::VeCoHanhLy(std::unique_ptr<VeMayBay> v, HanhLy hl)
    : VeDecorator(std::move(v)), hanhLy(hl) {}


double VeCoHanhLy::tinhGia() const { return ve->tinhGia() + hanhLy.tinhPhi(); }


std::string VeCoHanhLy::moTa() const { return ve->moTa() + " + " + hanhLy.moTa(); }
