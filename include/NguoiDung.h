#pragma once
#include "Common.h"

class NguoiDung {
    // [NGOAI UML] FileRepository can doc/ghi du lieu noi bo de luu file CSV
    friend class FileRepository;
public:
    NguoiDung(std::string ma, std::string ten, std::string mk);
    virtual ~NguoiDung();
    std::string getMaND() const;
    std::string getHoTen() const;
    bool dangNhap(std::string mk) const;
    virtual void hienMenu() = 0;
    virtual std::string vaiTro() const = 0;
protected:
    std::string maND;
    std::string hoTen;
    std::string matKhau;
};


// ----- Admin -----
