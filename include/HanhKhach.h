#pragma once
#include "Common.h"
#include "NguoiDung.h"

class HanhKhach : public NguoiDung {
public:
    HanhKhach(std::string ma, std::string ten, std::string mk, std::string cccd, std::string sdt, std::string email);
    std::string getCCCD() const;
    std::string getSoDienThoai() const;
    std::string getEmail() const;
    void hienMenu() override;
    std::string vaiTro() const override;
private:
    std::string cccd;
    std::string soDienThoai;
    std::string email;
};


// ----- SanBay -----
