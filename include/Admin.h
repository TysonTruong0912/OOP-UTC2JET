#pragma once
#include "Common.h"
#include "NguoiDung.h"

class Admin : public NguoiDung {
public:
    Admin(std::string ma, std::string ten, std::string mk);
    void hienMenu() override;
    std::string vaiTro() const override;
};


// ----- DaiLy -----
