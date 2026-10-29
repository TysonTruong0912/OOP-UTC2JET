// DichVuGiaTang.cpp
#include "DichVuGiaTang.h"

DichVuGiaTang::DichVuGiaTang(std::string ma, std::string ten) : maDV(std::move(ma)), ten(std::move(ten)) {}


DichVuGiaTang::~DichVuGiaTang() = default;


// ----- HanhLy.cpp -----
HanhLy::HanhLy(double kg, double donGiaKg)
    : DichVuGiaTang("HL", "Hanh ly"), soKg(kg), donGiaKg(donGiaKg) {}
