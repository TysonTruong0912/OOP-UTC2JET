// SoDoGhe.cpp
#include "SoDoGhe.h"

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
