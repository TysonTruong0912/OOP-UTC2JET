// Ghe.cpp
#include "Ghe.h"

        case HangGhe::THUONG_GIA: return soGheThuongGia;
        case HangGhe::PHO_THONG:  return soGhePhoThong;
        case HangGhe::TIET_KIEM:  return soGheTietKiem;
    }
    return 0;
}

int MayBay::tongSoGhe() const { return soGheThuongGia + soGhePhoThong + soGheTietKiem; }

        case HangGhe::PHO_THONG:  return soGhePhoThong;
        case HangGhe::TIET_KIEM:  return soGheTietKiem;
    }
    return 0;
}

int MayBay::tongSoGhe() const { return soGheThuongGia + soGhePhoThong + soGheTietKiem; }

        case HangGhe::TIET_KIEM:  return soGheTietKiem;
    }
    return 0;
}

int MayBay::tongSoGhe() const { return soGheThuongGia + soGhePhoThong + soGheTietKiem; }

Ghe::Ghe(std::string so, HangGhe h) : soGhe(std::move(so)), hang(h), trangThai(TrangThaiGhe::TRONG) {}


std::string Ghe::getSoGhe() const { return soGhe; }

HangGhe Ghe::getHang() const { return hang; }

TrangThaiGhe Ghe::getTrangThai() const { return trangThai; }

void Ghe::setTrangThai(TrangThaiGhe tt) { trangThai = tt; }

        case HangGhe::THUONG_GIA: return "THUONG GIA";
        case HangGhe::PHO_THONG:  return "PHO THONG";
        case HangGhe::TIET_KIEM:  return "TIET KIEM";
    }
    return "";
}
}

// Sinh ghe tu cau hinh may bay: thuong gia 4 ghe/hang (A-D), pho thong va tiet kiem 6 ghe/hang (A-F).
SoDoGhe::SoDoGhe(const MayBay& mb) {
    const HangGhe cacHang[] = {HangGhe::THUONG_GIA, HangGhe::PHO_THONG, HangGhe::TIET_KIEM};
    const int gheMoiHang[] = {4, 6, 6};
    int dayGhe = 1;
    for (int k = 0; k < 3; ++k) {
        const int tong = mb.getSoGhe(cacHang[k]);
        for (int i = 0; i < tong; ++i) {
            if (i > 0 && i % gheMoiHang[k] == 0) ++dayGhe;
            const char cot = static_cast<char>('A' + (i % gheMoiHang[k]));
            dsGhe.emplace_back(std::to_string(dayGhe) + cot, cacHang[k]);
        }
        if (tong > 0) ++dayGhe;
    }
}

        case HangGhe::PHO_THONG:  return "PHO THONG";
        case HangGhe::TIET_KIEM:  return "TIET KIEM";
    }
    return "";
}
}

// Sinh ghe tu cau hinh may bay: thuong gia 4 ghe/hang (A-D), pho thong va tiet kiem 6 ghe/hang (A-F).
SoDoGhe::SoDoGhe(const MayBay& mb) {
    const HangGhe cacHang[] = {HangGhe::THUONG_GIA, HangGhe::PHO_THONG, HangGhe::TIET_KIEM};
    const int gheMoiHang[] = {4, 6, 6};
    int dayGhe = 1;
    for (int k = 0; k < 3; ++k) {
        const int tong = mb.getSoGhe(cacHang[k]);
        for (int i = 0; i < tong; ++i) {
            if (i > 0 && i % gheMoiHang[k] == 0) ++dayGhe;
            const char cot = static_cast<char>('A' + (i % gheMoiHang[k]));
            dsGhe.emplace_back(std::to_string(dayGhe) + cot, cacHang[k]);
        }
        if (tong > 0) ++dayGhe;
    }
}

        case HangGhe::TIET_KIEM:  return "TIET KIEM";
    }
    return "";
}
}

// Sinh ghe tu cau hinh may bay: thuong gia 4 ghe/hang (A-D), pho thong va tiet kiem 6 ghe/hang (A-F).
SoDoGhe::SoDoGhe(const MayBay& mb) {
    const HangGhe cacHang[] = {HangGhe::THUONG_GIA, HangGhe::PHO_THONG, HangGhe::TIET_KIEM};
    const int gheMoiHang[] = {4, 6, 6};
    int dayGhe = 1;
    for (int k = 0; k < 3; ++k) {
        const int tong = mb.getSoGhe(cacHang[k]);
        for (int i = 0; i < tong; ++i) {
            if (i > 0 && i % gheMoiHang[k] == 0) ++dayGhe;
            const char cot = static_cast<char>('A' + (i % gheMoiHang[k]));
            dsGhe.emplace_back(std::to_string(dayGhe) + cot, cacHang[k]);
        }
        if (tong > 0) ++dayGhe;
    }
}

SoDoGhe::SoDoGhe(const MayBay& mb) {
    const HangGhe cacHang[] = {HangGhe::THUONG_GIA, HangGhe::PHO_THONG, HangGhe::TIET_KIEM};
    const int gheMoiHang[] = {4, 6, 6};
    int dayGhe = 1;
    for (int k = 0; k < 3; ++k) {
        const int tong = mb.getSoGhe(cacHang[k]);
        for (int i = 0; i < tong; ++i) {
            if (i > 0 && i % gheMoiHang[k] == 0) ++dayGhe;
            const char cot = static_cast<char>('A' + (i % gheMoiHang[k]));
            dsGhe.emplace_back(std::to_string(dayGhe) + cot, cacHang[k]);
        }
        if (tong > 0) ++dayGhe;
    }
}


Ghe* SoDoGhe::timGhe(std::string soGhe) {
    for (Ghe& g : dsGhe) if (g.getSoGhe() == soGhe) return &g;
    return nullptr;
}


std::vector<Ghe*> SoDoGhe::layGheTrong(HangGhe h) {
    std::vector<Ghe*> kq;
    for (Ghe& g : dsGhe)
        if (g.getHang() == h && g.getTrangThai() == TrangThaiGhe::TRONG) kq.push_back(&g);
    return kq;
}


int SoDoGhe::demGheTrong(HangGhe h) const {
    int dem = 0;
    for (const Ghe& g : dsGhe)
        if (g.getHang() == h && g.getTrangThai() == TrangThaiGhe::TRONG) ++dem;
    return dem;
}


bool SoDoGhe::giuGhe(std::string soGhe) {
    Ghe* g = timGhe(soGhe);
    if (!g || g->getTrangThai() != TrangThaiGhe::TRONG) return false;
    g->setTrangThai(TrangThaiGhe::GIU_CHO);
    return true;
}


void SoDoGhe::huyGiu(std::string soGhe) {
    Ghe* g = timGhe(soGhe);
    if (g && g->getTrangThai() == TrangThaiGhe::GIU_CHO) g->setTrangThai(TrangThaiGhe::TRONG);
}

bool SoDoGhe::datGhe(std::string soGhe) {
    Ghe* g = timGhe(soGhe);
    if (!g || g->getTrangThai() == TrangThaiGhe::DA_DAT) return false;
    g->setTrangThai(TrangThaiGhe::DA_DAT);
    return true;
}


void SoDoGhe::traGhe(std::string soGhe) {
    Ghe* g = timGhe(soGhe);
    if (g) g->setTrangThai(TrangThaiGhe::TRONG);
}


void SoDoGhe::hienThiSoDo() const {
    std::cout << "  Chu thich: [ ] trong   [H] dang giu   [X] da dat\n";
    const HangGhe cacHang[] = {HangGhe::THUONG_GIA, HangGhe::PHO_THONG, HangGhe::TIET_KIEM};
    for (HangGhe h : cacHang) {
        std::cout << "  " << tenHang(h) << " (còn " << demGheTrong(h) << " ghe trong)\n   ";
        int dem = 0;
        const int moiDong = (h == HangGhe::THUONG_GIA) ? 4 : 6;
        for (const Ghe& g : dsGhe) {
            if (g.getHang() != h) continue;
            const char* kyHieu = g.getTrangThai() == TrangThaiGhe::TRONG ? "[ ]"
                                : g.getTrangThai() == TrangThaiGhe::GIU_CHO ? "[H]" : "[X]";
            std::cout << ' ' << g.getSoGhe() << kyHieu;
            if (++dem % moiDong == 0) std::cout << "\n   ";
        }
        std::cout << '\n';
    }
}

        case HangGhe::THUONG_GIA: return std::make_unique<VeThuongGia>(std::move(maVe), std::move(soGhe), cb, hk);
        case HangGhe::PHO_THONG:  return std::make_unique<VePhoThong>(std::move(maVe), std::move(soGhe), cb, hk);
        case HangGhe::TIET_KIEM:  return std::make_unique<VeTietKiem>(std::move(maVe), std::move(soGhe), cb, hk);
    }
    throw LoiNghiepVu("Hang ghe khong hop le");
}


// ----- HanhTrinhBuilder.cpp -----
HanhTrinhBuilder::HanhTrinhBuilder() {}

        case HangGhe::PHO_THONG:  return std::make_unique<VePhoThong>(std::move(maVe), std::move(soGhe), cb, hk);
        case HangGhe::TIET_KIEM:  return std::make_unique<VeTietKiem>(std::move(maVe), std::move(soGhe), cb, hk);
    }
    throw LoiNghiepVu("Hang ghe khong hop le");
}


// ----- HanhTrinhBuilder.cpp -----
HanhTrinhBuilder::HanhTrinhBuilder() {}

        case HangGhe::TIET_KIEM:  return std::make_unique<VeTietKiem>(std::move(maVe), std::move(soGhe), cb, hk);
    }
    throw LoiNghiepVu("Hang ghe khong hop le");
}


// ----- HanhTrinhBuilder.cpp -----
HanhTrinhBuilder::HanhTrinhBuilder() {}

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
