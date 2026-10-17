#pragma once
// SanBay.h — San bay / May bay / Hang / Chuyen bay (P2_ChuyenBay)
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

class SanBay {
public:
    SanBay(std::string ma, std::string ten, std::string thanhPho);
    std::string getMaSB() const;
    std::string getTen() const;
    std::string getThanhPho() const;
private:
    std::string maSB;
    std::string ten;
    std::string thanhPho;
};
