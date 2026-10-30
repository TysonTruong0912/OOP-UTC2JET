#pragma once
#include "Common.h"
#include "VeMayBay.h"
#include "ChuyenBay.h"
#include "HanhKhach.h"
#include "QuanLyChuyenBay.h"

class QuanLyDatVe {
    // [NGOAI UML] FileRepository can doc/ghi du lieu noi bo de luu file CSV
    friend class FileRepository;
public:
    QuanLyDatVe(QuanLyChuyenBay& ql);
    GiuCho* giuCho(std::string maCB, std::string soGhe, HanhKhach* hk);
    VeMayBay* xacNhanThanhToan(GiuCho* gc, HangGhe h, DaiLy* dl);
    std::vector<VeMayBay*> datVeTheoHanhTrinh(const HanhTrinh& ht, DaiLy* dl);
    void huyVe(std::string maVe);
    void checkIn(std::string maVe, std::string soGheMoi);
    void inBoardingPass(std::string maVe) const;
    VeMayBay* timVe(std::string maVe) const;
    std::map<std::string, int> thongKeLoaiVe() const;
    double tinhDoanhThu() const;
    // [BO SUNG UX] liet ke ve
    std::vector<VeMayBay*> layDsVe() const;
private:
    std::vector<std::unique_ptr<VeMayBay> > dsVe;
    QuanLyChuyenBay& qlChuyenBay;
};


// ----- QuanLyNguoiDung -----
