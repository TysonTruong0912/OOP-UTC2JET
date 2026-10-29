// SuatAn.cpp
#include "SuatAn.h"

SuatAn::SuatAn(std::string mon, double gia) : DichVuGiaTang("SA", "Suat an"), monAn(std::move(mon)), gia(gia) {}


double SuatAn::tinhPhi() const { return gia; }

std::string SuatAn::moTa() const {
    std::ostringstream os;
    os << std::setprecision(10) << "Suat an: " << monAn << " (" << gia << ")";
    return os.str();
}

VeCoSuatAn::VeCoSuatAn(std::unique_ptr<VeMayBay> v, SuatAn sa)
    : VeDecorator(std::move(v)), suatAn(sa) {}


double VeCoSuatAn::tinhGia() const { return ve->tinhGia() + suatAn.tinhPhi(); }


std::string VeCoSuatAn::moTa() const { return ve->moTa() + " + " + suatAn.moTa(); }
