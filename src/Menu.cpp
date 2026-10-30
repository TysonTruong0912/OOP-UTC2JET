#include "AirlineSystem.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <stdexcept>
#include <regex>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;
namespace fs = std::filesystem;

namespace {
// UML cua Menu khong co thuoc tinh "nguoi dang nhap" nen giu o day trong luc menu chay.
NguoiDung* nguoiHienTai = nullptr;

// ---------- nhap xuat ----------
std::string cat(std::string s) {
    const char* trang = " \t\r\n";
    s.erase(0, s.find_first_not_of(trang));
    s.erase(s.find_last_not_of(trang) + 1);
    return s;
}

std::string docDong(const std::string& hoi) {
    std::cout << hoi << std::flush;
    std::string s;
    if (!std::getline(std::cin, s)) return "";
    return cat(s);
}

std::string lamSachNhap(std::string s) {          // du lieu luu CSV: bo , ; va dau " + " (dung de tach dich vu)
    for (char& c : s) if (c == ',' || c == ';' || c == '\n' || c == '\r') c = ' ';
    s = cat(s);
    const std::string cach = " + ";
    for (std::size_t vt = s.find(cach); vt != std::string::npos; vt = s.find(cach)) s.replace(vt, cach.size(), " & ");
    return s;
}

int docSo(const std::string& hoi) {
    while (std::cin) {
        const std::string s = docDong(hoi);
        if (s.empty()) {
            std::cout << "Vui lòng nhập số nguyên (không để trống).\n";
            continue;
        }
        try {
            std::size_t pos = 0;
            const int v = std::stoi(s, &pos);
            if (pos != s.size()) throw std::invalid_argument("thua ky tu");
            return v;
        } catch (...) {
            if (std::cin) std::cout << "Sai: chỉ nhập số nguyên (vd 1, 2, 10). Không nhập chữ.\n";
        }
    }
    return 0;
}

// So nguyen >= 0 (tu choi so am, chu)
int docSoKhongAm(const std::string& hoi) {
    while (std::cin) {
        const int v = docSo(hoi);
        if (!std::cin) return 0;
        if (v < 0) {
            std::cout << "Không chấp nhận số âm. Vui lòng nhập lại.\n";
            continue;
        }
        return v;
    }
    return 0;
}

// So nguyen > 0
int docSoDuong(const std::string& hoi) {
    while (std::cin) {
        const int v = docSo(hoi);
        if (!std::cin) return 0;
        if (v <= 0) {
            std::cout << "Phải nhập số nguyên dương (> 0). Vui lòng nhập lại.\n";
            continue;
        }
        return v;
    }
    return 0;
}

int docSoTrongKhoang(const std::string& hoi, int lo, int hi) {
    while (std::cin) {
        const int v = docSo(hoi);
        if (!std::cin) return lo;
        if (v >= lo && v <= hi) return v;
        std::cout << "Lựa chọn ngoài phạm vi. Nhập số từ " << lo << " den " << hi << ".\n";
    }
    return lo;
}

double docSoThuc(const std::string& hoi) {
    while (std::cin) {
        const std::string s = docDong(hoi);
        if (s.empty()) {
            std::cout << "Vui lòng nhập số (không để trống).\n";
            continue;
        }
        try {
            std::size_t pos = 0;
            const double v = std::stod(s, &pos);
            if (pos != s.size()) throw std::invalid_argument("thua ky tu");
            return v;
        } catch (...) {
            if (std::cin) std::cout << "Sai: chỉ nhập số (vd 1500000). Không nhập chữ.\n";
        }
    }
    return 0;
}

double docSoThucDuong(const std::string& hoi) {
    while (std::cin) {
        const double v = docSoThuc(hoi);
        if (!std::cin) return 0;
        if (v <= 0) {
            std::cout << "Giá / số lượng phải > 0. Vui lòng nhập lại.\n";
            continue;
        }
        return v;
    }
    return 0;
}

// Email bat buoc: co @, co ten mien va dau cham (vd ten@gmail.com)
bool chiChuSo(const std::string& s) {
    if (s.empty()) return false;
    for (unsigned char c : s) if (!std::isdigit(c)) return false;
    return true;
}

bool emailHopLe(const std::string& email) {
    if (email.empty() || email.find(' ') != std::string::npos) return false;
    const auto at = email.find('@');
    if (at == std::string::npos || at == 0 || at + 1 >= email.size()) return false;
    if (email.find('@', at + 1) != std::string::npos) return false; // chi 1 @
    const std::string domain = email.substr(at + 1);
    const auto dot = domain.find('.');
    if (dot == std::string::npos || dot == 0 || dot + 1 >= domain.size()) return false;
    // ky tu cuoi khong duoc la dau cham
    if (domain.back() == '.') return false;
    // local part khong rong
    if (at < 1) return false;
    return true;
}

std::string docEmail(const std::string& hoi = "Email: ") {
    while (std::cin) {
        const std::string e = lamSachNhap(docDong(hoi));
        if (emailHopLe(e)) return e;
        std::cout << "Email không hợp lệ / không đúng định dạng.\n"
                  << "  Bắt buộc có @ và tên miền (vd: tenban@gmail.com, ten@yahoo.com).\n"
                  << "  Nhập lại email.\n";
    }
    return "";
}

std::string gioNgan(time_point tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[32];
    std::strftime(buf, sizeof buf, "%d/%m %H:%M", &tm);
    return buf;
}

std::string dinhDangTien(double v) {
    std::string s = std::to_string(static_cast<long long>(v));
    for (int i = static_cast<int>(s.size()) - 3; i > 0; i -= 3) s.insert(static_cast<std::size_t>(i), ".");
    return s + " VND";
}

// Đếm ký tự hiển thị UTF-8 (không đếm từng byte) — căn cột console
int soKyTuHienThi(const std::string& s) {
    int n = 0;
    for (std::size_t i = 0; i < s.size(); ) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        if (c < 0x80) { ++n; ++i; }
        else if ((c >> 5) == 0x6) { ++n; i += 2; }
        else if ((c >> 4) == 0xE) { ++n; i += 3; }
        else if ((c >> 3) == 0x1E) { ++n; i += 4; }
        else { ++i; }
    }
    return n;
}
std::string canTrai(const std::string& s, int rong) {
    const int n = soKyTuHienThi(s);
    if (n >= rong) return s;
    return s + std::string(static_cast<std::size_t>(rong - n), ' ');
}
std::string canPhai(const std::string& s, int rong) {
    const int n = static_cast<int>(s.size()); // tiền ASCII + dấu chấm
    if (n >= rong) return s;
    return std::string(static_cast<std::size_t>(rong - n), ' ') + s;
}

bool docGio(const std::string& hoi, time_point& kq) {      // "dd/mm/yyyy hh:mm"
    const std::string s = docDong(hoi);
    int d, mo, y, h, mi;
    if (std::sscanf(s.c_str(), "%d/%d/%d %d:%d", &d, &mo, &y, &h, &mi) != 5) return false;
    std::tm tm{};
    tm.tm_mday = d; tm.tm_mon = mo - 1; tm.tm_year = y - 1900; tm.tm_hour = h; tm.tm_min = mi; tm.tm_isdst = -1;
    kq = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return true;
}

time_point docNgayLoc(const std::string& hoi) {            // "dd/mm/yyyy" hoac Enter = bat ky ngay
    const std::string s = docDong(hoi);
    int d, mo, y;
    if (std::sscanf(s.c_str(), "%d/%d/%d", &d, &mo, &y) != 3) return time_point{};
    std::tm tm{};
    tm.tm_mday = d; tm.tm_mon = mo - 1; tm.tm_year = y - 1900; tm.tm_hour = 12; tm.tm_isdst = -1;
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

HangGhe chonHang() {
    std::cout << " Hạng vé:\n"
              << "  1. Thương gia (x2.5)\n"
              << "  2. Phổ thông (x1.0)\n"
              << "  3. Tiết kiệm (x0.8)\n";
    while (std::cin) {
        const int c = docSoTrongKhoang("Chọn hạng vé (1-3): ", 1, 3);
        if (c == 1) return HangGhe::THUONG_GIA;
        if (c == 2) return HangGhe::PHO_THONG;
        if (c == 3) return HangGhe::TIET_KIEM;
        std::cout << "  Lựa chọn không hợp lệ, nhập lại.\n";
    }
    return HangGhe::PHO_THONG;
}

void inDanhSachSanBay(const std::vector<SanBay*>& ds) {
    if (ds.empty()) { std::cout << "  (chưa có sân bay nào)\n"; return; }
    std::cout << std::left << "  " << std::setw(4) << "STT" << std::setw(8) << "Mã"
              << std::setw(22) << "Ten" << "Thanh pho\n";
    for (std::size_t i = 0; i < ds.size(); ++i)
        std::cout << "  " << std::setw(4) << (i + 1) << std::setw(8) << ds[i]->getMaSB()
                  << std::setw(22) << ds[i]->getTen() << ds[i]->getThanhPho() << "\n";
}

void inDanhSachMayBay(const std::vector<MayBay*>& ds) {
    if (ds.empty()) { std::cout << "  (chua co may bay nao)\n"; return; }
    std::cout << std::left << "  " << std::setw(4) << "STT" << std::setw(12) << "So hieu"
              << std::setw(10) << "TG" << std::setw(10) << "PT" << "TK\n";
    for (std::size_t i = 0; i < ds.size(); ++i)
        std::cout << "  " << std::setw(4) << (i + 1) << std::setw(12) << ds[i]->getSoHieu()
                  << std::setw(10) << ds[i]->getSoGhe(HangGhe::THUONG_GIA)
                  << std::setw(10) << ds[i]->getSoGhe(HangGhe::PHO_THONG)
                  << ds[i]->getSoGhe(HangGhe::TIET_KIEM) << "\n";
}

void inDanhSach(const std::vector<ChuyenBay*>& ds) {
    if (ds.empty()) { std::cout << "  (không có chuyến bay phù hợp)\n"; return; }
    std::cout << std::left << "  " << std::setw(4) << "STT" << std::setw(8) << "Mã"
              << std::setw(13) << "Tuyến" << std::setw(13) << "Giờ đi"
              << std::setw(13) << "Giờ đến" << std::setw(18) << "Giá cơ bản" << "Ghế trống TG/PT/TK\n";
    for (std::size_t i = 0; i < ds.size(); ++i) {
        ChuyenBay* cb = ds[i];
        cb->donDepGiuCho();
        SoDoGhe& sd = cb->getSoDoGhe();
        std::cout << "  " << std::setw(4) << (i + 1) << std::setw(8) << cb->getMaCB()
                  << std::setw(13) << (cb->getSanBayDi()->getMaSB() + "->" + cb->getSanBayDen()->getMaSB())
                  << std::setw(13) << gioNgan(cb->getGioDi()) << std::setw(13) << gioNgan(cb->getGioDen())
                  << std::setw(18) << dinhDangTien(cb->getGiaCoBan())
                  << sd.demGheTrong(HangGhe::THUONG_GIA) << "/" << sd.demGheTrong(HangGhe::PHO_THONG) << "/"
                  << sd.demGheTrong(HangGhe::TIET_KIEM) << "\n";
    }
}

// Chon san bay tu danh sach. choPhepTatCa=true: Enter / 0 = khong loc.
// Tra ve ma SB (hoac "" neu tat ca). throw neu huy.
std::string chonSanBayTuDs(QuanLyChuyenBay& ql, const std::string& tieuDe, bool choPhepTatCa) {
    const std::vector<SanBay*> ds = ql.layDsSanBay();
    std::cout << "\n--- " << tieuDe << " ---\n";
    inDanhSachSanBay(ds);
    if (choPhepTatCa) std::cout << "  0. Tất cả (khong loc)\n";
    while (std::cin) {
        const int stt = docSo(choPhepTatCa ? "Chọn STT sân bay (0 = tất cả): " : "Chọn STT sân bay: ");
        if (choPhepTatCa && stt == 0) return "";
        if (stt >= 1 && stt <= static_cast<int>(ds.size())) return ds[static_cast<std::size_t>(stt - 1)]->getMaSB();
        std::cout << "  STT khong hop le.\n";
    }
    return "";
}

MayBay* chonMayBayTuDs(QuanLyChuyenBay& ql) {
    const std::vector<MayBay*> ds = ql.layDsMayBay();
    std::cout << "\n--- Danh sach may bay ---\n";
    inDanhSachMayBay(ds);
    while (std::cin) {
        const int stt = docSo("Chọn STT may bay: ");
        if (stt >= 1 && stt <= static_cast<int>(ds.size())) return ds[static_cast<std::size_t>(stt - 1)];
        std::cout << "  STT khong hop le.\n";
    }
    return nullptr;
}

ChuyenBay* chonChuyenBayTuDs(const std::vector<ChuyenBay*>& ds, const std::string& tieuDe) {
    std::cout << "\n--- " << tieuDe << " ---\n";
    inDanhSach(ds);
    if (ds.empty()) return nullptr;
    while (std::cin) {
        const int stt = docSo("Chọn STT chuyen bay (0 = huy): ");
        if (stt == 0) return nullptr;
        if (stt >= 1 && stt <= static_cast<int>(ds.size())) return ds[static_cast<std::size_t>(stt - 1)];
        std::cout << "  STT khong hop le.\n";
    }
    return nullptr;
}

std::string chonGheTuSoDo(ChuyenBay* cb, const HangGhe* locHang = nullptr) {
    cb->donDepGiuCho();
    SoDoGhe& sd = cb->getSoDoGhe();
    sd.hienThiSoDo();
    // Thu thap ghe trong (co the loc theo hang)
    std::vector<Ghe*> trong;
    for (HangGhe h : {HangGhe::THUONG_GIA, HangGhe::PHO_THONG, HangGhe::TIET_KIEM}) {
        if (locHang && h != *locHang) continue;
        auto t = sd.layGheTrong(h);
        trong.insert(trong.end(), t.begin(), t.end());
    }
    if (trong.empty()) {
        std::cout << "  (khong con ghe trong)\n";
        return "";
    }
    std::cout << "  Ghế trong: ";
    for (std::size_t i = 0; i < trong.size(); ++i) {
        if (i) std::cout << ", ";
        std::cout << (i + 1) << "." << trong[i]->getSoGhe();
    }
    std::cout << "\n";
    while (std::cin) {
        const int stt = docSo("Chọn STT ghe (hoac 0 de nhap tay so ghe): ");
        if (stt == 0) {
            return docDong("Nhap so ghe (vd 3A): ");
        }
        if (stt >= 1 && stt <= static_cast<int>(trong.size()))
            return trong[static_cast<std::size_t>(stt - 1)]->getSoGhe();
        std::cout << "  STT khong hop le.\n";
    }
    return "";
}

HanhKhach* chonHanhKhachTuDs(QuanLyNguoiDung& ql) {
    std::vector<HanhKhach*> ds;
    for (NguoiDung* nd : ql.layDsNguoiDung())
        if (auto* hk = dynamic_cast<HanhKhach*>(nd)) ds.push_back(hk);
    if (ds.empty()) { std::cout << "  (chua co hanh khach)\n"; return nullptr; }
    std::cout << "\n--- Danh sach hanh khach ---\n";
    for (std::size_t i = 0; i < ds.size(); ++i)
        std::cout << "  " << (i + 1) << ". " << ds[i]->getMaND() << " - " << ds[i]->getHoTen()
                  << " (" << ds[i]->getSoDienThoai() << ")\n";
    while (std::cin) {
        const int stt = docSo("Chọn STT hanh khach: ");
        if (stt >= 1 && stt <= static_cast<int>(ds.size())) return ds[static_cast<std::size_t>(stt - 1)];
        std::cout << "  STT khong hop le.\n";
    }
    return nullptr;
}

void inVe(const VeMayBay* ve) {
    std::cout << "  " << ve->moTa() << " | Chuyến " << ve->getChuyenBay()->getMaCB() << " | Ghế " << ve->getSoGhe()
              << " | Gia " << dinhDangTien(ve->tinhGia()) << (ve->isDaCheckIn() ? " | da check-in" : "") << "\n";
}

void inDanhSachVe(const std::vector<VeMayBay*>& ds) {
    if (ds.empty()) { std::cout << "  (khong co ve)\n"; return; }
    for (std::size_t i = 0; i < ds.size(); ++i) {
        std::cout << "  " << (i + 1) << ". ";
        inVe(ds[i]);
    }
}

VeMayBay* veCuaToi(QuanLyDatVe& dv, HanhKhach* hk) {
    std::vector<VeMayBay*> cuaToi;
    for (VeMayBay* v : dv.layDsVe())
        if (!hk || v->getHanhKhach() == hk) cuaToi.push_back(v);
    if (cuaToi.empty()) throw LoiNghiepVu("Ban chua co ve nao");
    std::cout << "\n--- Ve cua ban ---\n";
    inDanhSachVe(cuaToi);
    while (std::cin) {
        const int stt = docSo("Chọn STT ve (0 = nhap ma ve): ");
        if (stt == 0) {
            VeMayBay* ve = dv.timVe(docDong("Nhap ma ve: "));
            if (!ve) throw LoiNghiepVu("Khong tim thay ve nay");
            if (hk && ve->getHanhKhach() != hk) throw LoiNghiepVu("Ve nay khong phai cua ban");
            return ve;
        }
        if (stt >= 1 && stt <= static_cast<int>(cuaToi.size())) {
            VeMayBay* ve = cuaToi[static_cast<std::size_t>(stt - 1)];
            if (hk && ve->getHanhKhach() != hk) throw LoiNghiepVu("Ve nay khong phai cua ban");
            return ve;
        }
        std::cout << "  STT khong hop le.\n";
    }
    throw LoiNghiepVu("Huy chon ve");
}


void chiTietSanBay(QuanLyChuyenBay& ql) {
    const std::vector<SanBay*> ds = ql.layDsSanBay();
    inDanhSachSanBay(ds);
    if (ds.empty()) return;
    const int stt = docSo("Chọn STT san bay de xem chi tiet: ");
    if (stt < 1 || stt > static_cast<int>(ds.size())) { std::cout << "STT khong hop le.\n"; return; }
    SanBay* sb = ds[static_cast<std::size_t>(stt - 1)];
    std::cout << "\n=== CHI TIET SAN BAY " << sb->getMaSB() << " - " << sb->getTen()
              << " (" << sb->getThanhPho() << ") ===\n";

    std::vector<ChuyenBay*> lienQuan;
    std::set<MayBay*> mayBaySet;
    for (ChuyenBay* cb : ql.layDsChuyenBay()) {
        if (cb->getSanBayDi() == sb || cb->getSanBayDen() == sb) {
            lienQuan.push_back(cb);
            if (cb->getMayBay()) mayBaySet.insert(cb->getMayBay());
        }
    }
    std::cout << "\n-- Chuyến bay di/den tai day (" << lienQuan.size() << ") --\n";
    inDanhSach(lienQuan);

    std::cout << "\n-- May bay tung hoat dong tai san bay nay --\n";
    if (mayBaySet.empty()) std::cout << "  (chua co)\n";
    else {
        std::vector<MayBay*> mb(mayBaySet.begin(), mayBaySet.end());
        inDanhSachMayBay(mb);
    }

    std::cout << "\n-- Hang hang khong trong he thong --\n";
    auto dsHang = ql.layDsHang();
    if (dsHang.empty()) std::cout << "  (chua co hang nao)\n";
    for (HangHangKhong* h : dsHang) {
        std::cout << "  " << h->getMaHang() << " - " << h->getTen()
                  << " | so chuyen dang quan ly: " << h->soChuyenBay() << "\n";
    }
    std::cout << "  (Hang co chuyen di/den san bay nay duoc suy ra tu danh sach chuyen bay o tren.)\n";
}

// ========== XUAT FILE HTML (giong demo sap xep: C++ ghi file, mở bằng trình duyệt) ==========
std::string htmlEsc(const std::string& s) {
    std::string o;
    for (char c : s) {
        if (c == '&') o += "&amp;";
        else if (c == '<') o += "&lt;";
        else if (c == '>') o += "&gt;";
        else if (c == '"') o += "&quot;";
        else o += c;
    }
    return o;
}

void xuatTrangHtml(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, const std::string& tenFile = "datve.html") {
    std::ofstream f(tenFile);
    if (!f) {
        std::cout << "Khong ghi duoc file " << tenFile << "\n";
        return;
    }
    auto dsCb = ql.layDsChuyenBay();
    ql.sapXepTheoGio(dsCb);
    auto dsSb = ql.layDsSanBay();
    auto dsMb = ql.layDsMayBay();
    auto dsHang = ql.layDsHang();
    auto dsVe = dv.layDsVe();

    f << R"(<!DOCTYPE html>
<html lang="vi"><head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>Airline System Management — Báo cáo</title>
<style>
:root{--bg:#eef2f7;--card:#fff;--tx:#0f172a;--mut:#64748b;--line:#e2e8f0;--pri:#1d4ed8;--ok:#15803d;--bad:#b91c1c;--tg:#7c3aed}
*{box-sizing:border-box}
body{margin:0;font:15px/1.55 "Segoe UI",system-ui,-apple-system,sans-serif;background:var(--bg);color:var(--tx)}
header{background:linear-gradient(135deg,#1e3a8a 0%,#1d4ed8 55%,#3b82f6 100%);color:#fff;padding:28px 24px;box-shadow:0 4px 20px rgba(30,58,138,.25)}
header h1{margin:0;font-size:1.5rem;font-weight:700;letter-spacing:-.02em}
header p{margin:8px 0 0;opacity:.92;font-size:.95rem;max-width:720px;line-height:1.45}
main{max-width:1120px;margin:24px auto;padding:0 16px 40px}
.card{background:var(--card);border:1px solid var(--line);border-radius:14px;padding:20px 22px;margin-bottom:16px;box-shadow:0 1px 3px rgba(15,23,42,.04)}
h2{margin:0 0 14px;font-size:1.1rem;font-weight:650;color:#1e293b;padding-bottom:10px;border-bottom:2px solid #e2e8f0}
table{width:100%;border-collapse:separate;border-spacing:0;font-size:14px}
th,td{padding:10px 12px;border-bottom:1px solid var(--line);text-align:left;vertical-align:middle}
th{color:#475569;font-size:11px;font-weight:700;text-transform:uppercase;letter-spacing:.04em;background:#f8fafc}
tr:last-child td{border-bottom:none}
tr:hover td{background:#f8fafc}
.tag{display:inline-block;padding:3px 10px;border-radius:999px;font-size:12px;font-weight:600;white-space:nowrap}
.ok{background:#dcfce7;color:var(--ok)}.het{background:#fee2e2;color:var(--bad)}
.tg{background:#ede9fe;color:var(--tg)}.pt{background:#dbeafe;color:var(--pri)}.tk{background:#dcfce7;color:var(--ok)}
.stat{display:grid;grid-template-columns:repeat(auto-fit,minmax(150px,1fr));gap:12px;margin-bottom:18px}
.stat .card{margin:0;text-align:center;padding:18px 12px}
.stat .mut{display:block;margin-bottom:6px;font-size:.8rem;text-transform:uppercase;letter-spacing:.03em}
.stat b{font-size:1.45rem;display:block;color:#1e3a8a;font-weight:700}
.mut{color:var(--mut);font-size:.9rem}
.right{text-align:right}
footer{text-align:center;color:var(--mut);font-size:13px;padding:24px 16px 32px;line-height:1.5}
@media (max-width:640px){header{padding:20px 16px}th,td{padding:8px 6px;font-size:13px}}
</style></head><body>
<header>
  <h1>Airline System Management — Báo cáo tình trạng hệ thống</h1>
  <p>File HTML do chương trình C++ ghi lại sau mỗi lần chọn «Xuất web HTML». Mở lại file (hoặc F5) để thấy dữ liệu mới.</p>
</header>
<main>
)";

    // Stats
    f << "<div class=\"stat\">"
      << "<div class=\"card\"><span class=\"mut\">Sân bay</span><b>" << dsSb.size() << "</b></div>"
      << "<div class=\"card\"><span class=\"mut\">Chuyến bay</span><b>" << dsCb.size() << "</b></div>"
      << "<div class=\"card\"><span class=\"mut\">Máy bay</span><b>" << dsMb.size() << "</b></div>"
      << "<div class=\"card\"><span class=\"mut\">Hãng HK</span><b>" << dsHang.size() << "</b></div>"
      << "<div class=\"card\"><span class=\"mut\">Vé đã bán</span><b>" << dsVe.size() << "</b></div>"
      << "<div class=\"card\"><span class=\"mut\">Doanh thu</span><b>" << htmlEsc(dinhDangTien(dv.tinhDoanhThu())) << "</b></div>"
      << "</div>\n";

    // Uu dai
    f << "<div class=\"card\"><h2>Ưu đãi hạng vé &amp; hành lý</h2><table><tr><th>Hạng</th><th>Hệ số</th><th>HL miễn phí</th><th>Ưu đãi</th></tr>"
      << "<tr><td><span class=\"tag tg\">Thương gia</span></td><td>x2.5</td><td>40kg + 10kg xách tay</td><td>Suất ăn kèm, đổi chỗ OK, phòng chờ TG</td></tr>"
      << "<tr><td><span class=\"tag pt\">Phổ thông</span></td><td>x1.0</td><td>20kg + 7kg xách tay</td><td>Suất ăn thêm phí, đổi chỗ cùng hạng</td></tr>"
      << "<tr><td><span class=\"tag tk\">Tiết kiệm</span></td><td>x0.8</td><td>15kg + 7kg xách tay</td><td>Không đổi vé/chỗ, giá thấp</td></tr>"
      << "</table><p class=\"mut\">Phí vượt mức: 50.000 VND/kg</p></div>\n";

    // San bay
    f << "<div class=\"card\"><h2>Danh sách sân bay</h2><table><tr><th>Mã</th><th>Tên</th><th>Thành phố</th></tr>";
    for (SanBay* sb : dsSb)
        f << "<tr><td><b>" << htmlEsc(sb->getMaSB()) << "</b></td><td>" << htmlEsc(sb->getTen())
          << "</td><td>" << htmlEsc(sb->getThanhPho()) << "</td></tr>";
    f << "</table></div>\n";

    // Chuyến bay + ghe
    f << "<div class=\"card\"><h2>Chuyến bay &amp; tình trạng ghế (CÒN VÉ / HẾT)</h2><table>"
      << "<tr><th>Mã</th><th>Tuyến</th><th>Giờ đi</th><th>Giờ đến</th><th>Giá CB</th><th>Máy bay</th>"
      << "<th>TG</th><th>PT</th><th>TK</th></tr>";
    for (ChuyenBay* cb : dsCb) {
        cb->donDepGiuCho();
        SoDoGhe& sd = cb->getSoDoGhe();
        MayBay* mb = cb->getMayBay();
        auto cell = [&](HangGhe h) {
            int c = sd.demGheTrong(h), t = mb->getSoGhe(h);
            std::string cls = c > 0 ? "ok" : "het";
            std::string lab = c > 0 ? "CÒN" : "HẾT";
            f << "<td><span class=\"tag " << cls << "\">" << c << "/" << t << " " << lab << "</span></td>";
        };
        f << "<tr><td><b>" << htmlEsc(cb->getMaCB()) << "</b></td><td>"
          << htmlEsc(cb->getSanBayDi()->getMaSB()) << " → " << htmlEsc(cb->getSanBayDen()->getMaSB())
          << "</td><td>" << htmlEsc(gioNgan(cb->getGioDi())) << "</td><td>" << htmlEsc(gioNgan(cb->getGioDen()))
          << "</td><td>" << htmlEsc(dinhDangTien(cb->getGiaCoBan())) << "</td><td>"
          << htmlEsc(mb->getSoHieu()) << "</td>";
        cell(HangGhe::THUONG_GIA);
        cell(HangGhe::PHO_THONG);
        cell(HangGhe::TIET_KIEM);
        f << "</tr>\n";
    }
    f << "</table></div>\n";

    // Ve
    f << "<div class=\"card\"><h2>Vé đã bán</h2>";
    if (dsVe.empty()) f << "<p class=\"mut\">Chưa có vé nào.</p>";
    else {
        f << "<table><tr><th>Mã vé</th><th>Hạng</th><th>Chuyến</th><th>Ghế</th><th>Hành khách</th><th>Giá</th><th>Check-in</th><th>Chi tiết</th></tr>";
        for (VeMayBay* v : dsVe) {
            f << "<tr><td><b>" << htmlEsc(v->getMaVe()) << "</b></td><td>" << htmlEsc(v->loaiVe())
              << "</td><td>" << htmlEsc(v->getChuyenBay()->getMaCB()) << "</td><td>" << htmlEsc(v->getSoGhe())
              << "</td><td>" << htmlEsc(v->getHanhKhach() ? v->getHanhKhach()->getHoTen() : "?")
              << "</td><td>" << htmlEsc(dinhDangTien(v->tinhGia())) << "</td><td>"
              << (v->isDaCheckIn() ? "<span class=\"tag ok\">Đã CI</span>" : "<span class=\"tag het\">Chưa</span>")
              << "</td><td class=\"mut\">" << htmlEsc(v->moTa()) << "</td></tr>\n";
        }
        f << "</table>";
    }
    f << "</div>\n";

    // Nguoi dung
    f << "<div class=\"card\"><h2>Người dùng</h2><table><tr><th>Mã</th><th>Họ tên</th><th>Vai trò</th><th>Thông tin</th></tr>";
    for (NguoiDung* u : nd.layDsNguoiDung()) {
        {
          std::string vt = u->vaiTro();
          if (vt == "Dai ly") vt = "Đại lý";
          else if (vt == "Hanh khach") vt = "Hành khách";
          f << "<tr><td>" << htmlEsc(u->getMaND()) << "</td><td>" << htmlEsc(u->getHoTen())
            << "</td><td>" << htmlEsc(vt) << "</td><td class=\"mut\">";
        }
        if (auto* dl = dynamic_cast<DaiLy*>(u))
            f << "Hoa hồng: " << htmlEsc(dinhDangTien(dl->getTongHoaHong()));
        if (auto* hk = dynamic_cast<HanhKhach*>(u))
            f << htmlEsc(hk->getSoDienThoai()) << " | " << htmlEsc(hk->getEmail());
        f << "</td></tr>\n";
    }
    f << "</table></div>\n";

    f << R"(</main>
<footer>Cập nhật từ BTL OOP DT04 — mỗi lần chọn «Xuất web HTML» trong menu, file này được ghi đè. Bấm F5 trên trình duyệt để xem bản mới.</footer>
</body></html>
)";
    f.close();
    std::cout << "Đã cập nhật file " << tenFile << " — mở bằng trình duyệt (hoặc F5 nếu đang mở).\n";
    std::cout << "  Đường dẫn: thư mục đang chạy / " << tenFile << "\n";
}



void luuTatCa(FileRepository& repo, QuanLyChuyenBay& cb, QuanLyDatVe& dv, QuanLyNguoiDung& nd) {
    repo.ghiNguoiDung(nd);
    repo.ghiChuyenBay(cb);
    repo.ghiVe(dv);
    std::cout << "Đã lưu dữ liệu.\n";
    xuatTrangHtml(cb, dv, nd, "datve.html");
}


void moWebTuMenu(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, FileRepository& repo) {
#if BAT_WEB_DEMO
    // Luu + xuat HTML bao cao, roi mo server web day du (dung CHUNG du lieu console)
    std::cout << "\n>>> Dang luu du lieu console va mo WEB DAY DU...\n";
    luuTatCa(repo, ql, dv, nd);

    int c = 8080;
    const std::string nhapCong = docDong("Cong web (Enter = 8080, hoac nhap so khac): ");
    if (!nhapCong.empty()) {
        try {
            int x = std::stoi(nhapCong);
            if (x > 0 && x < 65536) c = x;
        } catch (...) {}
    }

    if (!web::batDauTuConsole(ql, dv, nd, repo, c)) {
        std::cout << "Khong mo duoc web. Thu cong khac (vd 8081) hoac tat chuong trinh dang chiem cong.\n";
        return;
    }

    const std::string urlLocal = "http://localhost:" + std::to_string(c);
    std::cout << "\n========================================\n"
              << "  WEB ĐẦY ĐỦ đang chạy (Airline System Management)\n"
              << "  May tinh:     " << urlLocal << "\n";
    for (const std::string& ip : web::layDiaChiLan()) {
        std::cout << "  Dien thoai:   http://" << ip << ":" << c << "\n"
                  << "  (cung WiFi voi may tinh — go tren trinh duyet DT)\n";
        // ten mien gia lap de go de nho (sslip.io trỏ ve IP)
        std::string nip = ip;
        for (char& ch : nip) if (ch == '.') ch = '-';
        std::cout << "  Ten mien goi: http://" << nip << ".sslip.io:" << c << "\n";
    }
    std::cout << "========================================\n"
              << "  Dang nhap web:\n"
              << "    Quan tri vien        : AD01 / admin123\n"
              << "    Dai ly Sao Viet      : DL01 / daily123\n"
              << "    Vietnam Booking      : DL02 / daily123\n"
              << "    BestPrice Travel     : DL03 / daily123\n"
              << "    Sao Mai Travel       : DL04 / daily123\n"
              << "    Hong Ngoc Ha Travel  : DL05 / daily123\n"
              << "    Hanh khach           : tu dang ky tren web\n"
              << "  * Console van dung — KHONG tat cua so nay.\n"
              << "  * Khong cung WiFi / muon mo tren DT o xa:\n"
              << "    chay file  mo_web_cong_khai.bat  (can Cloudflared free)\n"
              << "    se co link https://....trycloudflare.com mo moi noi.\n"
              << "========================================\n\n";

    // Thu mo trinh duyet tu dong (Windows / macOS / Linux)
#ifdef _WIN32
    {
        std::string cmd = "cmd /c start \"\" \"" + urlLocal + "\"";
        std::system(cmd.c_str());
    }
#elif defined(__APPLE__)
    {
        std::string cmd = "open \"" + urlLocal + "\"";
        std::system(cmd.c_str());
    }
#else
    {
        std::string cmd = "xdg-open \"" + urlLocal + "\" >/dev/null 2>&1 &";
        std::system(cmd.c_str());
    }
#endif
#else
    (void)ql; (void)dv; (void)nd; (void)repo;
    std::cout << "Ban build khong bat BAT_WEB_DEMO - khong co web.\n";
#endif
}

// ---------- chuc nang dung chung ----------
void xuatMaTranNoiChuyen(QuanLyChuyenBay& ql, const std::string& path = "data/NOI_CHUYEN_CAC_TUYEN.txt");
void soSanhGiaMuaQuaDaiLy(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, HanhKhach* hk);

void timChuyen(QuanLyChuyenBay& ql) {
    std::cout << "\n(Dữ liệu nạp từ thư mục data/: sanbay.csv, chuyenbay.csv, maybay.csv, hang.csv)\n";
    std::cout << "Co " << ql.layDsSanBay().size() << " sân bay, "
              << ql.layDsChuyenBay().size() << " chuyến bay, "
              << ql.layDsMayBay().size() << " máy bay, "
              << ql.layDsHang().size() << " hãng hàng không.\n";
    const std::string di = chonSanBayTuDs(ql, "Sân bay / thành phố ĐI", true);
    const std::string den = chonSanBayTuDs(ql, "Sân bay / thành phố ĐẾN", true);
    const time_point ngay = docNgayLoc("Ngày bay dd/mm/yyyy (Enter = tất cả): ");
    std::vector<ChuyenBay*> ds = ql.timChuyenBay(di, den, ngay);
    std::cout << " Sắp xếp:\n  1. Theo giờ đi\n  2. Theo giá\n";
    if (docSo("Chọn: ") == 2) ql.sapXepTheoGia(ds); else ql.sapXepTheoGio(ds);
    std::cout << "\n=== DANH SACH CHUYEN BAY PHU HOP (" << ds.size() << " chuyến) ===\n";
    if (!di.empty() || !den.empty())
        std::cout << "  Lọc: " << (di.empty() ? "(mọi điểm đi)" : di) << " -> "
                  << (den.empty() ? "(mọi điểm đến)" : den) << "\n";
    inDanhSach(ds);
    if (!ds.empty())
        std::cout << "  -> Đặt vé: chọn menu Đặt vé / Dat theo hanh trinh, chon STT hoac ma chuyen.\n";
}


void timNoiChuyen(QuanLyChuyenBay& ql) {
    const std::string di = chonSanBayTuDs(ql, "Sân bay ĐI (bat buoc)", false);
    const std::string den = chonSanBayTuDs(ql, "Sân bay ĐẾN (bat buoc)", false);
    const time_point ngay = docNgayLoc("Ngày bay dd/mm/yyyy (Enter = tất cả / theo giờ thực tế): ");
    const auto kq = ql.timNoiChuyen(di, den, ngay, std::chrono::minutes(60), 3);
    if (kq.empty()) {
        std::cout << "  (khong co hanh trinh noi chuyen — thu doi diem di/den hoac de ngay trong)\n";
        return;
    }
    std::cout << "\n=== DANH SACH PHUONG AN NOI CHUYEN (" << kq.size() << " phuong an) ===\n";
    int stt = 0;
    for (const auto& duong : kq) {
        double tong = 0;
        std::cout << "\n--- Phuong an " << ++stt << " (" << duong.size() << " chang) ---\n";
        for (std::size_t i = 0; i < duong.size(); ++i) {
            ChuyenBay* cb = duong[i];
            cb->donDepGiuCho();
            SoDoGhe& sd = cb->getSoDoGhe();
            tong += cb->getGiaCoBan();
            std::cout << "  Chang " << (i + 1) << ": " << cb->getMaCB()
                      << " | " << cb->getSanBayDi()->getMaSB() << " -> " << cb->getSanBayDen()->getMaSB()
                      << " | " << cb->getSanBayDi()->getThanhPho() << " -> " << cb->getSanBayDen()->getThanhPho() << "\n"
                      << "           Giờ đi " << gioNgan(cb->getGioDi())
                      << " | Giờ đến " << gioNgan(cb->getGioDen())
                      << " | Gia " << dinhDangTien(cb->getGiaCoBan()) << "\n"
                      << "           Ghế trống TG/PT/TK: "
                      << sd.demGheTrong(HangGhe::THUONG_GIA) << "/"
                      << sd.demGheTrong(HangGhe::PHO_THONG) << "/"
                      << sd.demGheTrong(HangGhe::TIET_KIEM) << "\n";
            if (i + 1 < duong.size())
                std::cout << "           ~~ noi chuyen / cho tai "
                          << cb->getSanBayDen()->getMaSB() << " ~~\n";
        }
        std::cout << "  >> Tổng giá co ban: " << dinhDangTien(tong)
                  << " | Đặt vé: menu Đặt vé / Hanh trinh, chon tung ma chuyen tren.\n";
    }
    std::cout << "=============================================\n";
}

void xemSoDo(QuanLyChuyenBay& ql) {
    std::vector<ChuyenBay*> ds = ql.layDsChuyenBay();
    ql.sapXepTheoGio(ds);
    ChuyenBay* cb = chonChuyenBayTuDs(ds, "Chọn chuyến bay để xem sơ đồ ghế");
    if (!cb) return;
    cb->donDepGiuCho();
    std::cout << "Sơ đồ ghe chuyến " << cb->getMaCB() << ":\n";
    cb->getSoDoGhe().hienThiSoDo();
}

// Đặt vé: giu cho 5 phut -> xac nhan thanh toan. dl != nullptr khi dai ly dat ho khach.

// Muc hanh ly mien phi (kg ký gửi) theo hang ve
int hanhLyMienPhiKg(HangGhe h) {
    switch (h) {
        case HangGhe::THUONG_GIA: return 40;
        case HangGhe::PHO_THONG:  return 20;
        case HangGhe::TIET_KIEM:  return 15;
    }
    return 0;
}

std::string moTaUuDaiHang(HangGhe h) {
    switch (h) {
        case HangGhe::THUONG_GIA:
            return "x2.5 | HL miễn phí 40kg+10kg xách tay | suất ăn kèm | đổi chỗ OK | phòng chờ TG";
        case HangGhe::PHO_THONG:
            return "x1.0 | HL miễn phí 20kg+7kg xách tay | suất ăn thêm phí | đổi chỗ cùng hạng OK";
        case HangGhe::TIET_KIEM:
            return "x0.8 | HL miễn phí 15kg+7kg xách tay | suất ăn thêm phí | KHÔNG đổi vé/chỗ";
    }
    return "";
}

void inQuyDinhUuDaiVaHanhLy() {
    std::cout << "\n===== UU DAI HANG VE & QUY DINH HANH LY =====\n";
    for (HangGhe h : {HangGhe::THUONG_GIA, HangGhe::PHO_THONG, HangGhe::TIET_KIEM}) {
        const char* ten = h == HangGhe::THUONG_GIA ? "THUONG GIA" : h == HangGhe::PHO_THONG ? "PHO THONG" : "TIET KIEM";
        std::cout << "  [" << ten << "] mien phi " << hanhLyMienPhiKg(h) << " kg ky gui\n"
                  << "      " << moTaUuDaiHang(h) << "\n";
    }
    std::cout << "  Phí vượt mức: 50.000 VND/kg (ap dung phan kg vuot muc mien phi).\n"
              << "  File tham khao: data/UU_DAI_HANG_VE.txt , data/QUY_DINH_HANH_LY.txt\n";
    for (const char* ten : {"data/UU_DAI_HANG_VE.txt", "data/QUY_DINH_HANH_LY.txt"}) {
        std::ifstream f(ten);
        if (!f.is_open()) continue;
        std::cout << "\n----- " << ten << " -----\n";
        std::string dong;
        while (std::getline(f, dong)) std::cout << dong << "\n";
    }
}

void baoCaoTinhTrangChuyenBay(QuanLyChuyenBay& ql, bool ghiFile) {
    std::ostringstream bao;
    bao << "================================================================================\n"
        << "  TINH TRANG GHE / VE THEO CHUYEN BAY (cap nhat luc chay chuong trinh)\n"
        << "================================================================================\n";
    auto ds = ql.layDsChuyenBay();
    ql.sapXepTheoGio(ds);
    if (ds.empty()) bao << "  (chua co chuyen bay)\n";
    for (ChuyenBay* cb : ds) {
        cb->donDepGiuCho();
        SoDoGhe& sd = cb->getSoDoGhe();
        MayBay* mb = cb->getMayBay();
        const int tgT = sd.demGheTrong(HangGhe::THUONG_GIA);
        const int ptT = sd.demGheTrong(HangGhe::PHO_THONG);
        const int tkT = sd.demGheTrong(HangGhe::TIET_KIEM);
        const int tgTong = mb->getSoGhe(HangGhe::THUONG_GIA);
        const int ptTong = mb->getSoGhe(HangGhe::PHO_THONG);
        const int tkTong = mb->getSoGhe(HangGhe::TIET_KIEM);
        bao << "--------------------------------------------------------------------------------\n"
            << " Chuyến " << cb->getMaCB() << " | "
            << cb->getSanBayDi()->getMaSB() << "->" << cb->getSanBayDen()->getMaSB()
            << " | may bay " << mb->getSoHieu() << "\n"
            << " Giờ đi " << gioNgan(cb->getGioDi()) << " | Giờ đến " << gioNgan(cb->getGioDen())
            << " | Giá cơ bản " << dinhDangTien(cb->getGiaCoBan()) << "\n"
            << "  Khoang THUONG GIA : con " << tgT << "/" << tgTong << " ghe"
            << (tgT > 0 ? "  -> CON VE hang nay | " : "  -> HET VE hang nay | ")
            << moTaUuDaiHang(HangGhe::THUONG_GIA) << "\n"
            << "  Khoang PHO THONG  : con " << ptT << "/" << ptTong << " ghe"
            << (ptT > 0 ? "  -> CON VE hang nay | " : "  -> HET VE hang nay | ")
            << moTaUuDaiHang(HangGhe::PHO_THONG) << "\n"
            << "  Khoang TIET KIEM  : con " << tkT << "/" << tkTong << " ghe"
            << (tkT > 0 ? "  -> CON VE hang nay | " : "  -> HET VE hang nay | ")
            << moTaUuDaiHang(HangGhe::TIET_KIEM) << "\n";
        if (tgT + ptT + tkT == 0) bao << "  *** CHUYEN HET CHO ***\n";
    }
    bao << "================================================================================\n"
        << " Quy dinh hanh ly: TG 40kg | PT 20kg | TK 15kg mien phi; vuot muc 50.000 VND/kg\n"
        << " Chi tiet: data/UU_DAI_HANG_VE.txt , data/QUY_DINH_HANH_LY.txt\n";
    const std::string noiDung = bao.str();
    std::cout << noiDung;
    if (ghiFile) {
        fs::create_directories("data");
        std::ofstream f("data/TINH_TRANG_CHUYEN_BAY.txt");
        if (f) {
            f << noiDung;
            std::cout << "\nDa ghi file data/TINH_TRANG_CHUYEN_BAY.txt\n";
        } else {
            std::cout << "\nKhong ghi duoc file data/TINH_TRANG_CHUYEN_BAY.txt\n";
        }
    }
}

void canhBaoHanhLy(HangGhe h, double kgThem) {
    const int mien = hanhLyMienPhiKg(h);
    std::cout << "  Muc mien phi hang nay: " << mien << " kg ky gui.\n";
    if (kgThem <= 0) {
        std::cout << "  Khong mang them hanh ly vuot muc.\n";
        return;
    }
    const double phi = kgThem * 50000.0;
    std::cout << "  Vuot muc " << kgThem << " kg -> phi du kien "
              << dinhDangTien(phi) << " (50.000 VND/kg).\n";
}

void datVe(QuanLyChuyenBay& cbl, QuanLyDatVe& dv, HanhKhach* hk, DaiLy* dl) {
    std::vector<ChuyenBay*> ds = cbl.layDsChuyenBay();
    cbl.sapXepTheoGio(ds);
    ChuyenBay* cb = chonChuyenBayTuDs(ds, "Chọn chuyến bay để đặt vé");
    if (!cb) { std::cout << "Hủy đặt vé.\n"; return; }
    const std::string maCB = cb->getMaCB();
    const HangGhe h = chonHang();
    std::cout << "  Ưu đãi: " << moTaUuDaiHang(h) << "\n";
    std::cout << "  Còn " << cb->getSoDoGhe().demGheTrong(h) << " ghe hạng này; HL miễn phí "
              << hanhLyMienPhiKg(h) << " kg.\n";
    const std::string soGhe = chonGheTuSoDo(cb, &h);
    if (soGhe.empty()) { std::cout << "Huy dat ve (khong chon ghe).\n"; return; }
    GiuCho* gc = dv.giuCho(maCB, soGhe, hk);
    std::cout << "Da giu cho ghe " << soGhe << " trong 5 phut.\n";
    const std::string xn = docDong("Xac nhan thanh toan? (y/n): ");
    if (xn == "y" || xn == "Y") {
        VeMayBay* ve = dv.xacNhanThanhToan(gc, h, dl);
        std::cout << "Đặt vé thanh cong!\n";
        inVe(ve);
        // Cap nhat file HTML ngay sau khi dat ve
        // (ql/dv truyen vao ham datVe - can xuat o menu sau; o day chi thong bao)

    } else {
        cb->huyGiu(soGhe);
        std::cout << "Da huy giu cho.\n";
    }
}

// Đặt vé theo hành trình: dung HanhTrinhBuilder (nhieu chang + hanh ly + suat an).
void datVeHanhTrinh(QuanLyChuyenBay& cbl, QuanLyDatVe& dv, HanhKhach* hk, DaiLy* dl) {
    std::cout << "\n===== ĐẶT VÉ THEO HÀNH TRÌNH =====\n";
    std::cout << "Chọn điểm ĐI và ĐẾN — hệ thống liệt kê BAY THẲNG và/hoặc NỐI CHUYẾN để bạn chọn.\n";

    const std::string di = chonSanBayTuDs(cbl, "Sân bay / thành phố ĐI", false);
    const std::string den = chonSanBayTuDs(cbl, "Sân bay / thành phố ĐẾN", false);
    if (di.empty() || den.empty()) { std::cout << "Cần chọn điểm đi và đến.\n"; return; }
    if (di == den) { std::cout << "Điểm đi và đến trùng nhau.\n"; return; }

    const time_point ngay = docNgayLoc("Ngày bay dd/mm/yyyy (Enter = tất cả / gio thuc te): ");

    // 1) Bay thang
    std::vector<ChuyenBay*> thang = cbl.timChuyenBay(di, den, ngay);
    cbl.sapXepTheoGio(thang);

    // 2) Nối chuyến (2-3 chang)
    auto noi = cbl.timNoiChuyen(di, den, ngay, std::chrono::minutes(45), 3);

    if (thang.empty() && noi.empty()) {
        std::cout << "Không có chuyến thẳng hay nối chuyến phù hợp " << di << " -> " << den << ".\n";
        return;
    }

    // Bang lua chon tong hop: type 0 = thang (1 cb), type 1 = noi (nhieu cb)
    struct PA {
        int loai; // 0 thang, 1 noi
        std::vector<ChuyenBay*> chang;
        double tong{};
    };
    std::vector<PA> dsPA;
    for (ChuyenBay* cb : thang) {
        PA p; p.loai = 0; p.chang = {cb}; p.tong = cb->getGiaCoBan();
        dsPA.push_back(p);
    }
    for (const auto& duong : noi) {
        // bo qua neu trung 1 chang da co o bay thang (trung ma)
        if (duong.size() == 1) continue;
        PA p; p.loai = 1; p.chang = duong; p.tong = 0;
        for (ChuyenBay* cb : duong) p.tong += cb->getGiaCoBan();
        dsPA.push_back(p);
    }

    std::cout << "\n=== " << di << " -> " << den << " ===\n";
    if (!thang.empty() && noi.empty())
        std::cout << "Có thể bay thẳng — không cần nối chuyến. Chọn trong list:\n";
    else if (thang.empty() && !noi.empty())
        std::cout << "Cần nối chuyến. Chọn trong list:\n";
    else
        std::cout << "Có bay thẳng và nối chuyến. Chọn trong list:\n";

    std::cout << "\n  STT  Loại        Tuyến / lịch trình                          Giờ đi   Tổng giá\n";
    std::cout << "  ---------------------------------------------------------------------------\n";
    for (std::size_t i = 0; i < dsPA.size(); ++i) {
        const PA& p = dsPA[i];
        std::string tuyen;
        std::string gio;
        for (std::size_t k = 0; k < p.chang.size(); ++k) {
            ChuyenBay* cb = p.chang[k];
            if (k == 0) {
                tuyen = cb->getSanBayDi()->getMaSB();
                gio = gioNgan(cb->getGioDi());
            }
            tuyen += "->" + cb->getSanBayDen()->getMaSB();
            if (k + 1 < p.chang.size()) tuyen += "(đổi)";
        }
        // ma chuyen ngan
        std::string ma;
        for (std::size_t k = 0; k < p.chang.size(); ++k) {
            if (k) ma += "+";
            ma += p.chang[k]->getMaCB();
        }
        std::cout << "  " << std::setw(3) << (i + 1) << "  "
                  << std::left << std::setw(11) << (p.loai == 0 ? "Thẳng" : "Nối chuyến")
                  << std::setw(28) << (tuyen + " [" + ma + "]")
                  << "  " << gio << "  " << dinhDangTien(p.tong) << "\n";
    }
    std::cout << "\n  0. Hủy\n";
    const int chon = docSo("Chọn STT phuong an: ");
    if (chon <= 0 || chon > static_cast<int>(dsPA.size())) {
        std::cout << "Hủy đặt vé.\n";
        return;
    }
    const PA& pa = dsPA[static_cast<std::size_t>(chon - 1)];

    const HangGhe hangChon = chonHang();
    std::cout << "  Ưu đãi: " << moTaUuDaiHang(hangChon) << "\n";
    HanhTrinhBuilder b;
    b.chonHanhKhach(hk).chonHang(hangChon);
    for (ChuyenBay* cb : pa.chang) {
        cb->donDepGiuCho();
        const int con = cb->getSoDoGhe().demGheTrong(hangChon);
        if (con == 0) {
            std::cout << "  Chuyến " << cb->getMaCB() << " hết ghe hạng đã chọn. Hủy.\n";
            return;
        }
        std::cout << "  + Thêm chặng " << cb->getMaCB() << " (còn " << con << " ghe hạng này)\n";
        b.themChang(cb);
    }

    std::cout << "\n--- Hành lý (miễn phí " << hanhLyMienPhiKg(hangChon) << " kg ký gửi) ---\n";
    const double kg = docSoThuc("Số kg hành lý VƯỢT MỨC miễn phí (0 = khong): ");
    canhBaoHanhLy(hangChon, kg);
    if (kg > 0) b.themHanhLy(kg);

    // Suat an tu file thuc don
    std::cout << "\n--- Suất ăn (xem data/THUC_DON_SUAT_AN.txt) ---\n";
    {
        std::ifstream ftd("data/THUC_DON_SUAT_AN.txt");
        if (ftd) {
            std::string line;
            int in = 0;
            while (std::getline(ftd, line) && in < 35) {
                if (!line.empty() && (line[0] == ' ' || line[0] == '[' || (line[0] >= '0' && line[0] <= '9')))
                    std::cout << line << "\n";
                ++in;
            }
        } else {
            std::cout << "  (Không đọc được file thực đơn — nhập tay tên món + giá)\n";
        }
    }
    const int monStt = docSo("Chọn STT mon (0 = khong dat / tu nhap): ");
    if (monStt > 0) {
        // Bang gia mac dinh theo file
        static const char* tenMon[] = {
            "", "Com ga xoi mo", "Com suon/com tam", "Mi y bo", "Chao ga", "Banh mi thit",
            "Suat chay", "Banh quy/snack", "Hop trai cay", "Salad", "Sua chua",
            "Nuoc suoi", "Nuoc ngot", "Nuoc ep", "Ca phe", "Tra", "Bia lon",
            "Combo com+nuoc", "Combo mi+nuoc", "Combo snack+nuoc", "Suat thuong gia"
        };
        static const double giaMon[] = {
            0, 65000, 70000, 75000, 45000, 40000, 55000, 25000, 35000, 50000, 20000,
            15000, 25000, 35000, 30000, 25000, 45000, 85000, 90000, 40000, 180000
        };
        if (monStt >= 1 && monStt <= 20) {
            b.themSuatAn(tenMon[monStt], giaMon[monStt]);
            std::cout << "  Đã chọn: " << tenMon[monStt] << " — " << dinhDangTien(giaMon[monStt]) << "\n";
        } else {
            std::cout << "  STT không hợp lệ, bỏ qua suất ăn.\n";
        }
    } else {
        const std::string mon = lamSachNhap(docDong("Tên món (Enter = không): "));
        if (!mon.empty()) b.themSuatAn(mon, docSoThucDuong("Giá suất ăn (VND): "));
    }

    const HanhTrinh ht = b.build();
    const std::vector<VeMayBay*> ds = dv.datVeTheoHanhTrinh(ht, dl);
    double tong = 0;
    for (const VeMayBay* ve : ds) { inVe(ve); tong += ve->tinhGia(); }
    std::cout << "Đặt thành công " << ds.size() << " ve. Tổng tiền: " << dinhDangTien(tong) << "\n";
}




}  // namespace (Menu helpers)

Menu::Menu(QuanLyChuyenBay& qlCB, QuanLyDatVe& qlDV, QuanLyNguoiDung& qlND, FileRepository& repo)
    : qlChuyenBay(qlCB), qlDatVe(qlDV), qlNguoiDung(qlND), repo(repo) {}


void Menu::chay() {
    std::cout << "=== AIRLINE SYSTEM MANAGEMENT — ĐẶT VÉ & ĐIỀU HÀNH CHUYẾN BAY ===\n"
              << " Gợi ý: đăng nhập -> chọn MỞ WEB ĐẦY ĐỦ trong menu\n"
              << "   (Admin 11 / Đại lý 8 / Hành khách 10) để mở web đầy đủ.\n"
              << " Hoặc chạy:  chuong_trinh --web\n\n"
              << " --- Tài khoản có sẵn ---\n"
              << "  Quản trị viên     :  AD01 / admin123\n"
              << "  Đại lý Sao Việt   :  DL01 / daily123\n"
              << "  Vietnam Booking   :  DL02 / daily123\n"
              << "  BestPrice Travel  :  DL03 / daily123\n"
              << "  Sao Mai Travel    :  DL04 / daily123\n"
              << "  Hong Ngoc Ha Travel: DL05 / daily123\n"
              << "  Hành khách        :  tự đăng ký (menu 2)\n";
    while (std::cin) {
        std::cout << "\n 1. Đăng nhập\n 2. Đăng ký tài khoản HÀNH KHÁCH (tự tạo)\n 0. Thoát\n";
        const int c = docSo("Chọn: ");
        if (!std::cin) break;
        if (c < 0) {
            std::cout << "Không chấp nhận số âm. Vui lòng chọn 0, 1 hoặc 2.\n";
            continue;
        }
        if (c == 0) break;
        if (c != 1 && c != 2) {
            std::cout << "Lựa chọn không hợp lệ. Vui lòng chọn 0, 1 hoặc 2.\n";
            continue;
        }
        if (c == 2) {
            try {
                std::cout << "\n--- Đăng ký Hành khách ---\n";
                const std::string ma = lamSachNhap(docDong("Mã người dùng (vd HK03): "));
                if (ma.empty()) throw LoiNghiepVu("Mã không được để trống");
                if (ma.rfind("AD", 0) == 0 || ma.rfind("DL", 0) == 0)
                    throw LoiNghiepVu("Không dùng mã AD/DL. Chỉ đăng ký Hành khách (vd HK03)");
                if (qlNguoiDung.timNguoiDung(ma)) throw LoiNghiepVu("Mã đã tồn tại, chọn mã khác");
                const std::string ten = lamSachNhap(docDong("Họ tên: "));
                if (ten.empty()) throw LoiNghiepVu("Họ tên bắt buộc");
                const std::string mk = docDong("Mật khẩu: ");
                if (mk.size() < 3) throw LoiNghiepVu("Mật khẩu ít nhất 3 ký tự");
                const std::string cccd = lamSachNhap(docDong("CCCD (9 hoặc 12 số): "));
                if (cccd.empty()) throw LoiNghiepVu("CCCD bắt buộc");
                if (!((cccd.size() == 9 || cccd.size() == 12) && chiChuSo(cccd)))
                    throw LoiNghiepVu("CCCD không hợp lệ — phải 9 hoặc 12 chữ số");
                const std::string sdt = lamSachNhap(docDong("Số điện thoại: "));
                if (sdt.empty()) throw LoiNghiepVu("Số điện thoại bắt buộc");
                if (!((sdt.size() == 10 || sdt.size() == 11) && chiChuSo(sdt) && sdt[0] == '0'))
                    throw LoiNghiepVu("SĐT không hợp lệ — 10–11 số, bắt đầu bằng 0");
                const std::string email = docEmail("Email (vd ten@gmail.com): ");
                if (!emailHopLe(email)) throw LoiNghiepVu("Email không hợp lệ");
                qlNguoiDung.themNguoiDung(std::make_unique<HanhKhach>(ma, ten, mk, cccd, sdt, email));
                repo.ghiNguoiDung(qlNguoiDung);
                std::cout << "Đăng ký thành công! Hãy đăng nhập bằng mã " << ma << ".\n";
            } catch (const std::exception& e) {
                std::cout << "Loi dang ky: " << e.what() << "\n";
            }
            continue;
        }
        if (c != 1) continue;
        const std::string ma = lamSach(docDong("Mã người dùng: "));
        const std::string mk = docDong("Mật khẩu: ");
        // Dong bo tai khoan moi dang ky tren web (cung file data/nguoidung.csv)
        try { repo.docNguoiDung(qlNguoiDung); } catch (...) {}
        NguoiDung* nd = qlNguoiDung.dangNhap(ma, mk);
        if (!nd) {
            std::cout << "Sai mã người dùng hoặc mật khẩu.\n";
            std::cout << "  (Nếu vừa đăng ký trên web: kiểm tra mã/mật khẩu đúng, hoặc chạy lại app trong đúng thư mục có data/nguoidung.csv)\n";
            continue;
        }
        nguoiHienTai = nd;
        std::cout << "Xin chào " << nd->getHoTen() << " (" << nd->vaiTro() << ")\n";
        if (dynamic_cast<Admin*>(nd)) menuAdmin();
        else if (dynamic_cast<DaiLy*>(nd)) menuDaiLy();
        else if (dynamic_cast<HanhKhach*>(nd)) menuHanhKhach();
        nguoiHienTai = nullptr;
    }
    try { luuTatCa(repo, qlChuyenBay, qlDatVe, qlNguoiDung); }
    catch (const std::exception& e) { std::cout << "Khong luu duoc du lieu: " << e.what() << "\n"; }
}

void Menu::menuAdmin() {
    while (std::cin) {
        nguoiHienTai->hienMenu();
        const int c = docSo("Chọn: ");
        if (c == 0 || !std::cin) break;
        try {
            switch (c) {
                case 1: { // Quan ly chuyen bay
                    std::cout << "\n-- Quản lý chuyến bay --\n"
                              << " 1. Xem danh sách chuyến bay\n"
                              << " 2. Thêm chuyến bay\n"
                              << " 3. Xóa chuyến bay\n"
                              << " 0. Quay lai\n";
                    const int sc = docSo("Chọn: ");
                    if (sc == 1) {
                        std::vector<ChuyenBay*> ds = qlChuyenBay.timChuyenBay("", "", time_point{});
                        qlChuyenBay.sapXepTheoGio(ds);
                        inDanhSach(ds);
                    } else if (sc == 2) {
                        const std::string ma = lamSachNhap(docDong("Mã chuyến bay: "));
                        const std::string maDi = chonSanBayTuDs(qlChuyenBay, "Sân bay ĐI", false);
                        const std::string maDen = chonSanBayTuDs(qlChuyenBay, "Sân bay ĐẾN", false);
                        SanBay* di = qlChuyenBay.timSanBay(maDi);
                        SanBay* den = qlChuyenBay.timSanBay(maDen);
                        MayBay* mb = chonMayBayTuDs(qlChuyenBay);
                        if (ma.empty() || !di || !den || !mb || di == den)
                            throw LoiNghiepVu("Ma chuyen bay / san bay / may bay khong hop le");
                        time_point gioDi, gioDen;
                        if (!docGio("Giờ đi (dd/mm/yyyy hh:mm): ", gioDi) || !docGio("Giờ đến (dd/mm/yyyy hh:mm): ", gioDen))
                            throw LoiNghiepVu("Dinh dang gio khong dung");
                        if (gioDen <= gioDi) throw LoiNghiepVu("Giờ đến phai sau gio di");
                        const double gia = docSoThucDuong("Giá cơ bản (VND): ");
                        if (gia <= 0) throw LoiNghiepVu("Gia phai lon hon 0");
                        qlChuyenBay.themChuyenBay(std::make_unique<ChuyenBay>(ma, di, den, mb, gioDi, gioDen, gia));
                        std::cout << "Đã thêm chuyen bay " << ma << ".\n";
                    } else if (sc == 3) {
                        std::vector<ChuyenBay*> ds = qlChuyenBay.layDsChuyenBay();
                        qlChuyenBay.sapXepTheoGio(ds);
                        ChuyenBay* cb = chonChuyenBayTuDs(ds, "Chọn chuyến bay cần xóa");
                        if (!cb) { std::cout << "Hủy xóa.\n"; break; }
                        qlChuyenBay.xoaChuyenBay(cb->getMaCB());
                        std::cout << "Đã xóa chuyen bay.\n";
                    }
                    break;
                }
                case 2: { // Quan ly san bay / may bay
                    std::cout << "\n-- Quản lý sân bay / máy bay --\n"
                              << " 1. Xem danh sách sân bay / máy bay\n"
                              << " 2. Chi tiết sân bay\n"
                              << " 3. Thêm sân bay\n"
                              << " 4. Thêm máy bay\n"
                              << " 0. Quay lai\n";
                    const int sc = docSo("Chọn: ");
                    if (sc == 1) {
                        std::cout << "\n=== SÂN BAY ===\n";
                        inDanhSachSanBay(qlChuyenBay.layDsSanBay());
                        std::cout << "\n=== MÁY BAY ===\n";
                        inDanhSachMayBay(qlChuyenBay.layDsMayBay());
                    } else if (sc == 2) {
                        chiTietSanBay(qlChuyenBay);
                    } else if (sc == 3) {
                        const std::string ma = lamSachNhap(docDong("Mã sân bay: "));
                        const std::string ten = lamSachNhap(docDong("Tên sân bay: "));
                        const std::string tp = lamSachNhap(docDong("Thành phố: "));
                        if (ma.empty()) throw LoiNghiepVu("Ma san bay khong duoc de trong");
                        qlChuyenBay.themSanBay(std::make_unique<SanBay>(ma, ten, tp));
                        std::cout << "Đã thêm san bay " << ma << ".\n";
                        std::cout << "\n-- Cac hang hang khong hien co --\n";
                        for (HangHangKhong* h : qlChuyenBay.layDsHang())
                            std::cout << "  " << h->getMaHang() << " - " << h->getTen()
                                      << " (" << h->soChuyenBay() << " chuyen)\n";
                        std::cout << "-- May bay hien co --\n";
                        inDanhSachMayBay(qlChuyenBay.layDsMayBay());
                    } else if (sc == 4) {
                        const std::string soHieu = lamSachNhap(docDong("Số hiệu máy bay: "));
                        const std::string model = lamSachNhap(docDong("Model: "));
                        const int tg = docSo("Số ghe thương gia: ");
                        const int pt = docSo("Số ghe phổ thông: ");
                        const int tk = docSo("Số ghe tiết kiệm: ");
                        if (soHieu.empty() || tg < 0 || pt < 0 || tk < 0 || tg + pt + tk == 0)
                            throw LoiNghiepVu("Thong tin may bay khong hop le");
                        qlChuyenBay.themMayBay(std::make_unique<MayBay>(soHieu, model, tg, pt, tk));
                        std::cout << "Đã thêm may bay " << soHieu << ".\n";
                    }
                    break;
                }
                case 3:
                    std::cout << "Tổng doanh thu vé: " << dinhDangTien(qlDatVe.tinhDoanhThu()) << "\n";
                    break;
                case 4:
                    for (const auto& kv : qlDatVe.thongKeLoaiVe())
                        std::cout << "  " << kv.first << ": " << kv.second << " ve\n";
                    break;
                case 5:
                    luuTatCa(repo, qlChuyenBay, qlDatVe, qlNguoiDung);
                    break;
                case 6: {
                    std::cout << "\n=== TẤT CẢ VÉ ===\n";
                    inDanhSachVe(qlDatVe.layDsVe());
                    break;
                }
                case 7: {
                    std::cout << "\n=== NGƯỜI DÙNG ===\n";
                    for (NguoiDung* nd : qlNguoiDung.layDsNguoiDung()) {
                        std::cout << "  " << nd->getMaND() << " | " << nd->getHoTen()
                                  << " | " << nd->vaiTro();
                        if (auto* dlx = dynamic_cast<DaiLy*>(nd))
                            std::cout << " | hoa hồng " << dinhDangTien(dlx->getTongHoaHong());
                        if (auto* hk = dynamic_cast<HanhKhach*>(nd))
                            std::cout << " | " << hk->getSoDienThoai() << " | " << hk->getEmail();
                        std::cout << "\n";
                    }
                    break;
                }
                case 8:
                    xuatTrangHtml(qlChuyenBay, qlDatVe, qlNguoiDung, "datve.html");
                    break;
                case 9:
                    baoCaoTinhTrangChuyenBay(qlChuyenBay, true);
                    break;
                case 10:
                    inQuyDinhUuDaiVaHanhLy();
                    break;
                case 11:
                    moWebTuMenu(qlChuyenBay, qlDatVe, qlNguoiDung, repo);
                    break;

                case 12: {
                    std::cout << "\n=== QUẢN LÝ ĐẠI LÝ (hoa hồng theo ngày thực tế) ===\n";
                    // Cột cố định — canTrai/canPhai (UTF-8) để tên có dấu không lệch số
                    std::cout << "  " << canTrai("STT", 4) << canTrai("Mã", 6) << canTrai("Tên đại lý", 24)
                              << canPhai("TL%", 6) << "  " << canPhai("HH hôm nay", 16)
                              << "  " << canPhai("Tổng HH", 16) << "  " << canTrai("So với hôm qua", 22) << "\n";
                    std::cout << "  " << std::string(96, '-') << "\n";
                    int stt = 0;
                    double tongHH = 0, tongNgay = 0;
                    for (NguoiDung* nd : qlNguoiDung.layDsNguoiDung()) {
                        auto* dl = dynamic_cast<DaiLy*>(nd);
                        if (!dl) continue;
                        dl->dongBoNgayThuc();
                        ++stt;
                        tongHH += dl->getTongHoaHong();
                        tongNgay += dl->getHoaHongHomNay();
                        const double cl = dl->getChenhLechVoiHomQua();
                        std::ostringstream tyLe;
                        tyLe << static_cast<int>(dl->getTyLeHoaHong() * 100 + 0.5) << "%";
                        const std::string clStr = std::string(cl >= 0 ? "+" : "") + dinhDangTien(cl);
                        std::cout << "  " << canTrai(std::to_string(stt), 4)
                                  << canTrai(dl->getMaND(), 6)
                                  << canTrai(dl->getHoTen(), 24)
                                  << canPhai(tyLe.str(), 6) << "  "
                                  << canPhai(dinhDangTien(dl->getHoaHongHomNay()), 16) << "  "
                                  << canPhai(dinhDangTien(dl->getTongHoaHong()), 16) << "  "
                                  << canTrai(clStr, 18) << "\n";
                    }
                    if (stt == 0) std::cout << "  (chưa có đại lý)\n";
                    else {
                        std::cout << "  " << std::string(96, '-') << "\n";
                        std::cout << "  HH hôm nay (mọi ĐL): " << dinhDangTien(tongNgay)
                                  << "  |  Tổng HH: " << dinhDangTien(tongHH) << "\n";
                        std::cout << "  Doanh thu hệ thống (tất cả vé): " << dinhDangTien(qlDatVe.tinhDoanhThu()) << "\n";
                        std::cout << "  Tổng số vé đã bán: " << qlDatVe.layDsVe().size() << "\n";
                        std::cout << "  (Hoa hồng phản ánh mức độ bán qua từng đại lý — càng cao càng cạnh tranh tốt)\n";
                    }
                            break;
                }
                default:
                    std::cout << "Lựa chọn không hợp lệ (kể cả số âm). Vui lòng nhập đúng số trong menu.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Lỗi: " << e.what() << "\n";
        }
    }
}


void Menu::menuDaiLy() {
    DaiLy* dl = dynamic_cast<DaiLy*>(nguoiHienTai);
    while (std::cin) {
        nguoiHienTai->hienMenu();
        const int c = docSo("Chọn: ");
        if (c == 0 || !std::cin) break;
        try {
            switch (c) {
                case 1: timChuyen(qlChuyenBay); break;
                case 2: xemSoDo(qlChuyenBay); break;
                case 3: {
                    HanhKhach* hk = chonHanhKhachTuDs(qlNguoiDung);
                    if (!hk) throw LoiNghiepVu("Chua chon hanh khach");
                    datVe(qlChuyenBay, qlDatVe, hk, dl);
                    break;
                }
                case 4:
                    std::cout << "Tong hoa hong: " << dinhDangTien(dl->getTongHoaHong()) << "\n";
                    break;
                case 5:
                    xuatTrangHtml(qlChuyenBay, qlDatVe, qlNguoiDung, "datve.html");
                    break;
                case 6:
                    baoCaoTinhTrangChuyenBay(qlChuyenBay, true);
                    break;
                case 7:
                    inQuyDinhUuDaiVaHanhLy();
                    break;
                case 8:
                    moWebTuMenu(qlChuyenBay, qlDatVe, qlNguoiDung, repo);
                    break;
                default:
                    std::cout << "Lựa chọn không hợp lệ (kể cả số âm). Vui lòng nhập đúng số trong menu.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Lỗi: " << e.what() << "\n";
        }
    }
}

void Menu::menuHanhKhach() {
    HanhKhach* hk = dynamic_cast<HanhKhach*>(nguoiHienTai);
    while (std::cin) {
        nguoiHienTai->hienMenu();
        const int c = docSo("Chọn: ");
        if (c == 0 || !std::cin) break;
        try {
            switch (c) {
                case 1: timChuyen(qlChuyenBay); break;
                case 2: timNoiChuyen(qlChuyenBay); break;
                case 3: xemSoDo(qlChuyenBay); break;
                case 4: datVe(qlChuyenBay, qlDatVe, hk, nullptr); break;
                case 5: datVeHanhTrinh(qlChuyenBay, qlDatVe, hk, nullptr); break;
                case 6: {
                    VeMayBay* ve = veCuaToi(qlDatVe, hk);
                    ve->getChuyenBay()->donDepGiuCho();
                    ve->getChuyenBay()->getSoDoGhe().hienThiSoDo();
                    std::string ghe = docDong("Chon ghe moi (Enter = giu ghe hien tai " + ve->getSoGhe() + "): ");
                    if (ghe.empty()) ghe = ve->getSoGhe();
                    qlDatVe.checkIn(ve->getMaVe(), ghe);
                    std::cout << "Check-in thanh cong, ghe " << ve->getSoGhe() << ".\n";
                    break;
                }
                case 7:
                    qlDatVe.inBoardingPass(veCuaToi(qlDatVe, hk)->getMaVe());
                    break;
                case 8: {
                    const std::string ma = veCuaToi(qlDatVe, hk)->getMaVe();
                    qlDatVe.huyVe(ma);
                    std::cout << "Da huy ve " << ma << ", ghe duoc tra lai.\n";
                    break;
                }
                case 9:
                    xuatTrangHtml(qlChuyenBay, qlDatVe, qlNguoiDung, "datve.html");
                    break;
                case 10:
                    moWebTuMenu(qlChuyenBay, qlDatVe, qlNguoiDung, repo);
                    break;
                case 11:
                    soSanhGiaMuaQuaDaiLy(qlChuyenBay, qlDatVe, qlNguoiDung, hk);
                    break;
                default:
                    std::cout << "Lựa chọn không hợp lệ (kể cả số âm). Vui lòng nhập đúng số trong menu.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Lỗi: " << e.what() << "\n";
        }
    }
}
