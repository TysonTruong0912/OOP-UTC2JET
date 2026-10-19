// PHẦN 5 — Menu / File / Exception / Web
// Trích từ AirlineSystem.cpp — phần class phụ trách
#include "AirlineSystem.h"

// ----- FileRepository.cpp -----
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

// ----- Menu.cpp -----
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

// ----- LoiNghiepVu.cpp -----
LoiNghiepVu::LoiNghiepVu(std::string msg) : std::runtime_error(msg) {}

// ----- LoiHetCho.cpp -----
LoiHetCho::LoiHetCho(std::string msg) : LoiNghiepVu(std::move(msg)) {}

// ----- LoiGiuChoHetHan.cpp -----
LoiGiuChoHetHan::LoiGiuChoHetHan(std::string msg) : LoiNghiepVu(std::move(msg)) {}

// ----- LoiChuyenBayKhongTonTai.cpp -----
LoiChuyenBayKhongTonTai::LoiChuyenBayKhongTonTai(std::string msg) : LoiNghiepVu(std::move(msg)) {}

// ----- LoiDinhDangFile.cpp -----
LoiDinhDangFile::LoiDinhDangFile(std::string msg) : LoiNghiepVu(std::move(msg)) {}
