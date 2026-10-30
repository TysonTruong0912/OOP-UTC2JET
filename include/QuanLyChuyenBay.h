#pragma once
#include "Common.h"
#include "ChuyenBay.h"
#include "SanBay.h"
#include "MayBay.h"
#include "HangHangKhong.h"

class QuanLyChuyenBay {
    // [NGOAI UML] FileRepository can doc/ghi du lieu noi bo de luu file CSV
    friend class FileRepository;
public:
    void themChuyenBay(std::unique_ptr<ChuyenBay> cb);
    void xoaChuyenBay(std::string maCB);
    void themSanBay(std::unique_ptr<SanBay> sb);
    void themMayBay(std::unique_ptr<MayBay> mb);
    void themHang(std::unique_ptr<HangHangKhong> h);
    ChuyenBay* timChuyenBay(std::string maCB) const;
    SanBay* timSanBay(std::string maSB) const;
    MayBay* timMayBay(std::string soHieu) const;
    std::vector<ChuyenBay*> timChuyenBay(std::string di, std::string den, time_point ngay) const;
    std::vector<std::vector<ChuyenBay*> > timNoiChuyen(std::string di, std::string den, time_point ngay, std::chrono::minutes choToiThieu, int toiDaChang) const;
    void sapXepTheoGio(std::vector<ChuyenBay*>& ds);
    void sapXepTheoGia(std::vector<ChuyenBay*>& ds);
    // [BO SUNG UX] liet ke de menu/web hien danh sach chon (khong thay doi nghiep vu UML)
    std::vector<SanBay*> layDsSanBay() const;
    std::vector<MayBay*> layDsMayBay() const;
    std::vector<ChuyenBay*> layDsChuyenBay() const;
    std::vector<HangHangKhong*> layDsHang() const;
private:
    std::vector<std::unique_ptr<ChuyenBay> > dsChuyenBay;
    std::vector<std::unique_ptr<SanBay> > dsSanBay;
    std::vector<std::unique_ptr<MayBay> > dsMayBay;
    std::vector<std::unique_ptr<HangHangKhong> > dsHang;
};


// ----- QuanLyDatVe -----
