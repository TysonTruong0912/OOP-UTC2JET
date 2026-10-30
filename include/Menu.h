#pragma once
#include "Common.h"
#include "QuanLyChuyenBay.h"
#include "QuanLyDatVe.h"
#include "QuanLyNguoiDung.h"
#include "FileRepository.h"

class Menu {
public:
    Menu(QuanLyChuyenBay& qlCB, QuanLyDatVe& qlDV, QuanLyNguoiDung& qlND, FileRepository& repo);
    void chay();
private:
    QuanLyChuyenBay& qlChuyenBay;
    QuanLyDatVe& qlDatVe;
    QuanLyNguoiDung& qlNguoiDung;
    FileRepository& repo;
    void menuAdmin();
    void menuDaiLy();
    void menuHanhKhach();
};


// ----- LoiNghiepVu -----
