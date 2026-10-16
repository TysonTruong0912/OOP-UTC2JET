#pragma once
// FileRepository.h — Menu / File / Loi / Web (P5_MenuFileWeb)
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <chrono>
#include <iostream>
#include <stdexcept>

class SanBay; class MayBay; class ChuyenBay; class Ghe; class SoDoGhe; class GiuCho;
class HanhKhach; class DaiLy; class VeMayBay; class QuanLyChuyenBay; class QuanLyDatVe;
class QuanLyNguoiDung; class FileRepository; class NguoiDung; class HanhLy; class SuatAn;
using time_point = std::chrono::system_clock::time_point;

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
