// FileRepository.cpp — P5_MenuFileWeb
#include "FileRepository.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

FileRepository::FileRepository(std::string thuMuc) : thuMuc(std::move(thuMuc)) {}

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