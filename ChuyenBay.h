#pragma once
// ChuyenBay.h — San bay / May bay / Hang / Chuyen bay (P2_ChuyenBay)
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

class ChuyenBay {
public:
    ChuyenBay(std::string ma, SanBay* di, SanBay* den, MayBay* mb, time_point gioDi, time_point gioDen, double giaCoBan);
    std::string getMaCB() const;
    SanBay* getSanBayDi() const;
    SanBay* getSanBayDen() const;
    MayBay* getMayBay() const;
    time_point getGioDi() const;
    time_point getGioDen() const;
    double getGiaCoBan() const;
    SoDoGhe& getSoDoGhe();
    std::chrono::minutes thoiGianBay() const;
    GiuCho* giuGhe(std::string soGhe, HanhKhach* hk, std::chrono::seconds timeout);
    void huyGiu(std::string soGhe);
    void donDepGiuCho();
    void datGhe(std::string soGhe);
    void traGhe(std::string soGhe);
    bool operator<(const ChuyenBay& o) const;
    static bool soSanhTheoGia(const ChuyenBay& a, const ChuyenBay& b);
    void dichGio(std::chrono::seconds delta);
private:
    std::string maCB;
    SanBay* sanBayDi{};
    SanBay* sanBayDen{};
    MayBay* mayBay{};
    SoDoGhe soDoGhe;
    std::vector<std::unique_ptr<GiuCho> > dsGiuCho;
    std::chrono::system_clock::time_point gioDi{};
    std::chrono::system_clock::time_point gioDen{};
    double giaCoBan{};
};
