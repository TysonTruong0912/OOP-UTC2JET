#pragma once
// HangHangKhong.h — San bay / May bay / Hang / Chuyen bay (P2_ChuyenBay)
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include "Enums.h"

class SanBay; class MayBay; class ChuyenBay; class Ghe; class SoDoGhe; class GiuCho;
class HanhKhach; class DaiLy; class VeMayBay; class QuanLyChuyenBay; class QuanLyDatVe;
class QuanLyNguoiDung; class FileRepository; class NguoiDung; class HanhLy; class SuatAn;
using time_point = std::chrono::system_clock::time_point;

class HangHangKhong {
public:
    HangHangKhong(std::string ma, std::string ten);
    std::string getMaHang() const;
    std::string getTen() const;
    void themMayBay(MayBay* mb);
    void themChuyenBay(ChuyenBay* cb);
    int soChuyenBay() const;
private:
    std::string maHang;
    std::string ten;
    std::vector<MayBay*> dsMayBay;
    std::vector<ChuyenBay*> dsChuyenBay;
};
