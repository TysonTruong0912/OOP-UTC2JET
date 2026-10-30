#pragma once
#include "Common.h"
#include "QuanLyChuyenBay.h"
#include "QuanLyDatVe.h"
#include "QuanLyNguoiDung.h"

class FileRepository {
public:
    FileRepository(std::string thuMuc);
    void docChuyenBay(QuanLyChuyenBay& ql);
    void ghiChuyenBay(const QuanLyChuyenBay& ql);
    void docVe(QuanLyDatVe& ql);
    void ghiVe(const QuanLyDatVe& ql);
    void docNguoiDung(QuanLyNguoiDung& ql);
    void ghiNguoiDung(const QuanLyNguoiDung& ql);
private:
    std::string thuMuc;
};


// ----- Menu -----
