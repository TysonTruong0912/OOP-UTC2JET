#pragma once
// MayBay.h — San bay / May bay / Hang / Chuyen bay (P2_ChuyenBay)
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

class MayBay {
public:
    MayBay(std::string soHieu, std::string model, int gheTG, int ghePT, int gheTK);
    std::string getSoHieu() const;
    int getSoGhe(HangGhe h) const;
    int tongSoGhe() const;
private:
    std::string soHieu;
    std::string model;
    int soGheThuongGia{};
    int soGhePhoThong{};
    int soGheTietKiem{};
};
