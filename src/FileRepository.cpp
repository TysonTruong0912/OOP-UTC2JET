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
const double TY_LE_HOA_HONG_MAC_DINH = 0.05;

// UML khong co cach nao dua QuanLyNguoiDung vao docVe(), nen docNguoiDung() ghi nho no o day.
const QuanLyNguoiDung* nguoiDungDaDoc = nullptr;

std::string duongDan(const std::string& thuMuc, const std::string& ten) {
    return (fs::path(thuMuc) / ten).string();
}

std::string lamSach(std::string s) {            // CSV don gian: bo dau phay / cham phay / xuong dong
    for (char& c : s) if (c == ',' || c == ';' || c == '\n' || c == '\r') c = ' ';
    return s;
}

std::vector<std::string> tach(const std::string& s, char sep) {   // giu ca truong rong o cuoi
    std::vector<std::string> kq;
    std::size_t dau = 0;
    while (true) {
        std::size_t vt = s.find(sep, dau);
        if (vt == std::string::npos) { kq.push_back(s.substr(dau)); break; }
        kq.push_back(s.substr(dau, vt - dau));
        dau = vt + 1;
    }
    return kq;
}

long long giay(time_point tp) {
    return std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
}

time_point tuGiay(long long s) { return time_point(std::chrono::seconds(s)); }

void ghiFile(const std::string& thuMuc, const std::string& ten, const std::string& tieuDe,
             const std::function<void(std::ofstream&)>& ghi) {
    fs::create_directories(thuMuc);
    std::ofstream f(duongDan(thuMuc, ten));
    if (!f.is_open()) throw LoiDinhDangFile("Khong ghi duoc file " + duongDan(thuMuc, ten));
    f << tieuDe << '\n';
    ghi(f);
}

// Doc tung dong du lieu (bo dong tieu de). File khong ton tai thi bo qua.
void docFile(const std::string& thuMuc, const std::string& ten,
             const std::function<void(const std::vector<std::string>&)>& xuLy) {
    const std::string dd = duongDan(thuMuc, ten);
    std::ifstream f(dd);
    if (!f.is_open()) return;
    std::string dong;
    std::getline(f, dong);
    int so = 1;
    while (std::getline(f, dong)) {
        ++so;
        if (!dong.empty() && dong.back() == '\r') dong.pop_back();
        if (dong.empty()) continue;
        try {
            xuLy(tach(dong, ','));
        } catch (const LoiDinhDangFile& e) {
            throw LoiDinhDangFile(ten + " dong " + std::to_string(so) + ": " + e.what());
        } catch (const std::exception& e) {
            throw LoiDinhDangFile(ten + " dong " + std::to_string(so) + ": du lieu khong hop le (" + e.what() + ")");
        }
    }
}

void canTruong(const std::vector<std::string>& f, std::size_t n) {
    if (f.size() != n) throw LoiDinhDangFile("can " + std::to_string(n) + " truong, co " + std::to_string(f.size()));
}

std::vector<std::string> tachDichVu(const std::string& moTa) {      // "ma - hang + dv1 + dv2" -> {dv1, dv2}
    std::vector<std::string> kq;
    const std::string cach = " + ";
    std::size_t dau = moTa.find(cach);
    while (dau != std::string::npos) {
        std::size_t tiep = moTa.find(cach, dau + cach.size());
        kq.push_back(moTa.substr(dau + cach.size(), tiep == std::string::npos ? std::string::npos : tiep - dau - cach.size()));
        dau = tiep;
    }
    return kq;
}
}

FileRepository::FileRepository(std::string thuMuc) : thuMuc(std::move(thuMuc)) {}

// ---------------- CHUYEN BAY (kem san bay, may bay, hang) ----------------
void FileRepository::ghiChuyenBay(const QuanLyChuyenBay& ql) {
    ghiFile(thuMuc, "sanbay.csv", "maSB,ten,thanhPho", [&](std::ofstream& f) {
        for (const auto& sb : ql.dsSanBay)
            f << lamSach(sb->getMaSB()) << ',' << lamSach(sb->getTen()) << ',' << lamSach(sb->getThanhPho()) << '\n';
    });
    ghiFile(thuMuc, "maybay.csv", "soHieu,model,gheThuongGia,ghePhoThong,gheTietKiem", [&](std::ofstream& f) {
        for (const auto& mb : ql.dsMayBay)
            f << lamSach(mb->getSoHieu()) << ",-," << mb->getSoGhe(HangGhe::THUONG_GIA) << ','
              << mb->getSoGhe(HangGhe::PHO_THONG) << ',' << mb->getSoGhe(HangGhe::TIET_KIEM) << '\n';
    });
    ghiFile(thuMuc, "hang.csv", "maHang,ten", [&](std::ofstream& f) {
        for (const auto& h : ql.dsHang) f << lamSach(h->getMaHang()) << ',' << lamSach(h->getTen()) << '\n';
    });
    ghiFile(thuMuc, "chuyenbay.csv", "maCB,sanBayDi,sanBayDen,soHieuMayBay,gioDi,gioDen,giaCoBan", [&](std::ofstream& f) {
        for (const auto& cb : ql.dsChuyenBay)
            f << lamSach(cb->getMaCB()) << ',' << cb->getSanBayDi()->getMaSB() << ',' << cb->getSanBayDen()->getMaSB()
              << ',' << cb->getMayBay()->getSoHieu() << ',' << giay(cb->getGioDi()) << ',' << giay(cb->getGioDen())
              << ',' << static_cast<long long>(cb->getGiaCoBan()) << '\n';
    });
}

void FileRepository::docChuyenBay(QuanLyChuyenBay& ql) {
    ql.dsChuyenBay.clear();
    ql.dsSanBay.clear();
    ql.dsMayBay.clear();
    ql.dsHang.clear();
    docFile(thuMuc, "sanbay.csv", [&](const std::vector<std::string>& f) {
        canTruong(f, 3);
        ql.dsSanBay.push_back(std::make_unique<SanBay>(f[0], f[1], f[2]));
    });
    docFile(thuMuc, "maybay.csv", [&](const std::vector<std::string>& f) {
        canTruong(f, 5);
        ql.dsMayBay.push_back(std::make_unique<MayBay>(f[0], f[1], std::stoi(f[2]), std::stoi(f[3]), std::stoi(f[4])));
    });
    docFile(thuMuc, "hang.csv", [&](const std::vector<std::string>& f) {
        canTruong(f, 2);
        ql.dsHang.push_back(std::make_unique<HangHangKhong>(f[0], f[1]));
    });
    docFile(thuMuc, "chuyenbay.csv", [&](const std::vector<std::string>& f) {
        canTruong(f, 7);
        SanBay* di = ql.timSanBay(f[1]);
        SanBay* den = ql.timSanBay(f[2]);
        MayBay* mb = ql.timMayBay(f[3]);
        if (!di || !den || !mb) throw LoiDinhDangFile("chuyen bay " + f[0] + " tham chieu san bay / may bay khong ton tai");
        ql.dsChuyenBay.push_back(std::make_unique<ChuyenBay>(f[0], di, den, mb, tuGiay(std::stoll(f[4])),
                                                              tuGiay(std::stoll(f[5])), std::stod(f[6])));
    });
    // Gan lai may bay / chuyen bay vao hang theo tien to ma (VN/VJ/QH/VU...)
    for (auto& h : ql.dsHang) {
        const std::string ma = h->getMaHang();
        for (auto& mb : ql.dsMayBay) {
            // may bay thuong bat dau bang ma hang: VN-A001, VJ-B001...
            if (mb->getSoHieu().compare(0, ma.size(), ma) == 0) h->themMayBay(mb.get());
        }
        for (auto& cb : ql.dsChuyenBay) {
            if (cb->getMaCB().compare(0, ma.size(), ma) == 0) h->themChuyenBay(cb.get());
        }
    }
}

// ---------------- VE ----------------
void FileRepository::ghiVe(const QuanLyDatVe& ql) {
    ghiFile(thuMuc, "ve.csv", "maVe,loaiVe,maCB,soGhe,maND,daCheckIn,dichVu", [&](std::ofstream& f) {
        for (const auto& ve : ql.dsVe) {
            std::string dichVu;
            for (const std::string& dv : tachDichVu(ve->moTa())) dichVu += (dichVu.empty() ? "" : ";") + lamSach(dv);
            f << lamSach(ve->getMaVe()) << ',' << ve->loaiVe() << ',' << ve->getChuyenBay()->getMaCB() << ','
              << ve->getSoGhe() << ',' << ve->getHanhKhach()->getMaND() << ',' << (ve->isDaCheckIn() ? 1 : 0)
              << ',' << dichVu << '\n';
        }
    });
}

// Phai goi docNguoiDung() va docChuyenBay() truoc docVe().
void FileRepository::docVe(QuanLyDatVe& ql) {
    const std::regex hanhLy(R"(Hanh ly ([0-9.eE+\-]+)kg \(([0-9.eE+\-]+)/kg\))");
    const std::regex suatAn(R"(Suat an: (.*) \(([0-9.eE+\-]+)\))");
    ql.dsVe.clear();
    docFile(thuMuc, "ve.csv", [&](const std::vector<std::string>& f) {
        canTruong(f, 7);
        if (!nguoiDungDaDoc) throw LoiDinhDangFile("chua nap nguoi dung (goi docNguoiDung truoc docVe)");
        HangGhe hang;
        if (f[1] == "Thương gia") hang = HangGhe::THUONG_GIA;
        else if (f[1] == "Phổ thông") hang = HangGhe::PHO_THONG;
        else if (f[1] == "Tiết kiệm") hang = HangGhe::TIET_KIEM;
        else throw LoiDinhDangFile("loai ve la: " + f[1]);
        ChuyenBay* cb = ql.qlChuyenBay.timChuyenBay(f[2]);
        HanhKhach* hk = dynamic_cast<HanhKhach*>(nguoiDungDaDoc->timNguoiDung(f[4]));
        if (!cb || !hk || !cb->getSoDoGhe().timGhe(f[3]))
            throw LoiDinhDangFile("ve " + f[0] + " tham chieu chuyen bay / hanh khach / ghe khong ton tai");
        std::unique_ptr<VeMayBay> ve = VeFactory::taoVe(hang, f[0], f[3], cb, hk);
        for (const std::string& dv : tach(f[6], ';')) {
            if (dv.empty()) continue;
            std::smatch m;
            if (std::regex_match(dv, m, hanhLy))
                ve = std::make_unique<VeCoHanhLy>(std::move(ve), HanhLy(std::stod(m[1]), std::stod(m[2])));
            else if (std::regex_match(dv, m, suatAn))
                ve = std::make_unique<VeCoSuatAn>(std::move(ve), SuatAn(m[1], std::stod(m[2])));
            else
                throw LoiDinhDangFile("dich vu khong nhan ra: " + dv);
        }
        cb->getSoDoGhe().datGhe(f[3]);
        if (f[5] == "1") ve->danhDauCheckIn();
        ql.dsVe.push_back(std::move(ve));
    });
}

// ---------------- NGUOI DUNG ----------------
void FileRepository::ghiNguoiDung(const QuanLyNguoiDung& ql) {
    ghiFile(thuMuc, "nguoidung.csv", "vaiTro,maND,hoTen,matKhau,thongTin1,thongTin2,thongTin3", [&](std::ofstream& f) {
        for (const auto& nd : ql.dsNguoiDung) {
            f << nd->vaiTro() << ',' << lamSach(nd->getMaND()) << ',' << lamSach(nd->getHoTen()) << ','
              << lamSach(nd->matKhau);
            if (const HanhKhach* hk = dynamic_cast<const HanhKhach*>(nd.get()))
                f << ',' << lamSach(hk->getCCCD()) << ',' << lamSach(hk->getSoDienThoai()) << ',' << lamSach(hk->getEmail());
            else if (const DaiLy* dl = dynamic_cast<const DaiLy*>(nd.get()))
                f << ',' << dl->getTyLeHoaHong() << ',' << dl->getTongHoaHong() << ','
                  << dl->getHoaHongHomNay() << '|' << dl->getNgayHoaHong()
                  << '|' << dl->getHoaHongHomQua();
            else
                f << ",,,";
            f << '\n';
        }
    });
}

// Neu dai ly moi (tong HH = 0): gan so lieu mau thuc te theo ty le HH & ngay he thong
void ganHoaHongMauNeuTrong(QuanLyNguoiDung& ql) {
    const std::string homNay = []() {
        using clock = std::chrono::system_clock;
        const std::time_t t = clock::to_time_t(clock::now());
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        char buf[16]{};
        std::strftime(buf, sizeof buf, "%Y-%m-%d", &tm);
        return std::string(buf);
    }();
    // He so mau theo ma dai ly (tong tich luy ~ doanh so * ty le)
    const std::map<std::string, std::pair<double, double>> mau = {
        // ma -> {tongHoaHong, tyLeTrongNgay so voi tong de tinh HH hom nay}
        {"DL01", {2850000, 0.08}},
        {"DL02", {1920000, 0.06}},
        {"DL03", {1250000, 0.05}},
        {"DL04", {2100000, 0.07}},
        {"DL05", {3450000, 0.09}},
    };
    for (NguoiDung* nd : ql.layDsNguoiDung()) {
        auto* dl = dynamic_cast<DaiLy*>(nd);
        if (!dl) continue;
        dl->dongBoNgayThuc();
        if (dl->getTongHoaHong() > 0) continue;
        auto it = mau.find(dl->getMaND());
        if (it == mau.end()) {
            // dai ly moi them: tong 0, HH ngay 0 — dung
            continue;
        }
        const double tong = it->second.first;
        const double hhNgay = tong * it->second.second;
        dl->setTongHoaHong(tong);
        dl->setHoaHongHomNay(hhNgay, homNay);
    }
}


void FileRepository::docNguoiDung(QuanLyNguoiDung& ql) {
    ql.dsNguoiDung.clear();
    docFile(thuMuc, "nguoidung.csv", [&](const std::vector<std::string>& f) {
        canTruong(f, 7);
        if (f[0] == "Admin") {
            ql.dsNguoiDung.push_back(std::make_unique<Admin>(f[1], f[2], f[3]));
        } else if (f[0] == "Dai ly") {
            double tyLe = TY_LE_HOA_HONG_MAC_DINH;
            if (!f[4].empty()) {
                tyLe = std::stod(f[4]);
                if (tyLe > 1.0) tyLe /= 100.0;
            }
            auto dl = std::make_unique<DaiLy>(f[1], f[2], f[3], tyLe);
            if (f.size() > 5 && !f[5].empty()) {
                try { dl->setTongHoaHong(std::stod(f[5])); } catch (...) {}
            }
            if (f.size() > 6 && !f[6].empty()) {
                const std::string t3 = f[6];
                try {
                    const auto b1 = t3.find('|');
                    if (b1 == std::string::npos) {
                        dl->setHoaHongHomNay(std::stod(t3), "");
                    } else {
                        const auto b2 = t3.find('|', b1 + 1);
                        const double hhN = std::stod(t3.substr(0, b1));
                        if (b2 == std::string::npos) {
                            dl->setHoaHongHomNay(hhN, t3.substr(b1 + 1));
                        } else {
                            dl->setHoaHongHomNay(hhN, t3.substr(b1 + 1, b2 - b1 - 1));
                            dl->setHoaHongHomQua(std::stod(t3.substr(b2 + 1)));
                        }
                    }
                } catch (...) {}
            }
            dl->dongBoNgayThuc();
            ql.dsNguoiDung.push_back(std::move(dl));
        } else if (f[0] == "Hanh khach") {
            ql.dsNguoiDung.push_back(std::make_unique<HanhKhach>(f[1], f[2], f[3], f[4], f[5], f[6]));
        } else {
            throw LoiDinhDangFile("vai tro la: " + f[0]);
        }
    });
    nguoiDungDaDoc = &ql;
    ganHoaHongMauNeuTrong(ql);
}
