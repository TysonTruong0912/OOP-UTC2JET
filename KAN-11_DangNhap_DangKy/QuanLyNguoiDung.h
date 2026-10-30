#pragma once
#include "AirlineSystem.h"
#include "Enums.h"

class QuanLyNguoiDung;
class FileRepository;
class Menu;
class LoiNghiepVu;
class LoiHetCho;
class LoiGiuChoHetHan;
class LoiChuyenBayKhongTonTai;
class LoiDinhDangFile;
#if BAT_WEB_DEMO
namespace web {
bool batDauTuConsole(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, FileRepository& repo, int cong);
std::vector<std::string> layDiaChiLan();
}
