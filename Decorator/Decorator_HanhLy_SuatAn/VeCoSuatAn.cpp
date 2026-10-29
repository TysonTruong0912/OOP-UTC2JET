// VeCoSuatAn.cpp
#include "VeCoSuatAn.h"

VeCoSuatAn::VeCoSuatAn(std::unique_ptr<VeMayBay> v, SuatAn sa)
    : VeDecorator(std::move(v)), suatAn(sa) {}


double VeCoSuatAn::tinhGia() const { return ve->tinhGia() + suatAn.tinhPhi(); }


std::string VeCoSuatAn::moTa() const { return ve->moTa() + " + " + suatAn.moTa(); }
