#pragma once
#include "AirlineSystem.h"
#include "Enums.h"

class GiuCho;
class ChuyenBay;
class HanhTrinh;
class VeMayBay;
class VeThuongGia;
class VePhoThong;
class VeTietKiem;
class VeDecorator;
class VeCoHanhLy;
class VeCoSuatAn;
class DichVuGiaTang;
class HanhLy;
class SuatAn;
class VeFactory;
class HanhTrinhBuilder;
class QuanLyChuyenBay;
class QuanLyDatVe;
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
