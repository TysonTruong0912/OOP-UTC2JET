#pragma once
#include "Common.h"
#include "NguoiDung.h"
#include "Admin.h"
#include "DaiLy.h"
#include "HanhKhach.h"

class QuanLyNguoiDung {
    // [NGOAI UML] FileRepository can doc/ghi du lieu noi bo de luu file CSV
    friend class FileRepository;
public:
    void themNguoiDung(std::unique_ptr<NguoiDung> nd);
    NguoiDung* dangNhap(std::string ma, std::string mk) const;
    NguoiDung* timNguoiDung(std::string ma) const;
    // [BO SUNG UX] liet ke nguoi dung cho Admin/Dai ly
    std::vector<NguoiDung*> layDsNguoiDung() const;
private:
    std::vector<std::unique_ptr<NguoiDung> > dsNguoiDung;
};


// ----- FileRepository -----
