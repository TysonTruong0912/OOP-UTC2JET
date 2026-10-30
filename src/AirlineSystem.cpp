/**
 * AirlineSystem.cpp — cài đặt class + helpers + web demo
 * (tách từ PHAN 3 + PHAN 4 + PHAN 5)
 */
#include "AirlineSystem.h"

// PHAN 3: CAI DAT CAC LOP (noi dung truoc day nam trong cac file .cpp)
// ======================================================================

// ----- NguoiDung.cpp -----
NguoiDung::NguoiDung(std::string ma, std::string ten, std::string mk)
    : maND(std::move(ma)), hoTen(std::move(ten)), matKhau(std::move(mk)) {}

NguoiDung::~NguoiDung() = default;

std::string NguoiDung::getMaND() const { return maND; }
std::string NguoiDung::getHoTen() const { return hoTen; }
bool NguoiDung::dangNhap(std::string mk) const { return mk == matKhau; }


// ----- Admin.cpp -----
Admin::Admin(std::string ma, std::string ten, std::string mk)
    : NguoiDung(std::move(ma), std::move(ten), std::move(mk)) {}

void Admin::hienMenu() {
    std::cout << "\n===== MENU ADMIN =====\n"
              << " 1. Quản lý chuyến bay          (xem DS / thêm / xóa)\n"
              << " 2. Quản lý sân bay / máy bay   (xem DS / chi tiết sân bay / thêm sân bay / thêm máy bay)\n"
              << " 3. Xem doanh thu\n"
              << " 4. Thống kê vé theo hạng\n"
              << " 5. Lưu dữ liệu\n"
              << " 6. Xem tất cả vé\n"
              << " 7. Xem danh sách người dùng\n"
              << " 8. Xuất file WEB HTML (datve.html - báo cáo)\n"
              << " 9. Tình trạng ghế/vé từng chuyến (+ xuất file)\n"
              << "10. Quy định ưu đãi hạng vé & hành lý\n"
              << "11. *** MỞ WEB ĐẦY ĐỦ (console -> web, 3 vai trò) ***\n"
              << "12. Quản lý đại lý (doanh thu / hoa hồng / so sánh)\n"
              << " 0. Đăng xuất\n";
}

std::string Admin::vaiTro() const { return "Admin"; }


// ----- DaiLy.cpp -----
namespace {
std::string ngayHeThongHomNay() {
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
    return buf;
}
}

DaiLy::DaiLy(std::string ma, std::string ten, std::string mk, double tyLe)
    : NguoiDung(std::move(ma), std::move(ten), std::move(mk)),
      tyLeHoaHong(tyLe), tongHoaHong(0.0),
      hhBanVeHomNay(0.0), hhThiTruongHomNay(0.0), hhHomQua(0.0),
      ngayHoaHong(ngayHeThongHomNay()) {
    dongBoNgayThuc();
}

// Muc HH thi truong theo TUNG NGAY (lich may) — moi dai ly khac nhau;
// ngay moi co the TANG hoac GIAM so voi hom qua (khong phu thuoc sang/toi).
static double uocHoaHongThiTruong(const std::string& maDL, double tyLe, const std::string& ngay) {
    unsigned h = 2166136261u;
    for (unsigned char c : ngay) { h ^= c; h *= 16777619u; }
    for (unsigned char c : maDL) { h ^= c; h *= 16777619u; }
    // Muc co so theo ty le hoa hong
    const double base = 1200000.0 + tyLe * 32000000.0;  // vd 5% ~ 2.8tr / ngay
    // Bien dong ngay: -18% .. +18% (co ngay len, co ngay xuong)
    const double wave = (static_cast<int>(h % 2001) - 1000) / 1000.0;
    const double dayVal = base * (1.0 + 0.18 * wave);
    return dayVal > 0 ? dayVal : 0;
}

void DaiLy::dongBoNgayThuc() {
    const std::string homNay = ngayHeThongHomNay();
    const double marketNow = uocHoaHongThiTruong(getMaND(), tyLeHoaHong, homNay);

    if (ngayHoaHong != homNay) {
        // Sang ngay moi: chot moc hom qua, reset HH ban ve trong ngay, gan muc thi truong ngay moi
        if (ngayHoaHong.empty()) {
            hhHomQua = marketNow; // lan dau: lay moc tham chieu
        } else {
            hhHomQua = hhBanVeHomNay + hhThiTruongHomNay;
            if (hhHomQua <= 0)
                hhHomQua = uocHoaHongThiTruong(getMaND(), tyLeHoaHong, ngayHoaHong);
        }
        hhBanVeHomNay = 0.0;
        ngayHoaHong = homNay;
        hhThiTruongHomNay = marketNow;
        // Cap nhat tong theo muc thi truong ngay moi (phan in vao tong tich luy)
        tongHoaHong += marketNow * 0.25;
    } else {
        // Cung ngay: giu nguyen muc thi truong (khong tu tang vi "toi")
        if (hhThiTruongHomNay <= 0)
            hhThiTruongHomNay = marketNow;
        else
            hhThiTruongHomNay = marketNow; // dong bo lai cong thuc ngay (on dinh trong ngay)
    }
}

void DaiLy::congHoaHong(double giaTriVe) {
    dongBoNgayThuc();
    const double hh = giaTriVe * tyLeHoaHong;
    tongHoaHong += hh;
    hhBanVeHomNay += hh;
}

double DaiLy::getTongHoaHong() const { return tongHoaHong; }
double DaiLy::getHoaHongHomNay() const { return hhBanVeHomNay + hhThiTruongHomNay; }
double DaiLy::getHoaHongHomQua() const { return hhHomQua; }
double DaiLy::getHoaHongBanVeHomNay() const { return hhBanVeHomNay; }
double DaiLy::getHoaHongThiTruongHomNay() const { return hhThiTruongHomNay; }
double DaiLy::getChenhLechVoiHomQua() const { return getHoaHongHomNay() - hhHomQua; }
std::string DaiLy::getNgayHoaHong() const { return ngayHoaHong; }
void DaiLy::setTongHoaHong(double t) { tongHoaHong = t < 0 ? 0 : t; }
void DaiLy::setHoaHongHomQua(double h) { hhHomQua = h < 0 ? 0 : h; }
void DaiLy::setHoaHongHomNay(double h, const std::string& ngay) {
    // Tuong thich file cu: coi toan bo la phan thi truong + ban ve hop nhat
    hhThiTruongHomNay = h < 0 ? 0 : h;
    hhBanVeHomNay = 0;
    ngayHoaHong = ngay.empty() ? ngayHeThongHomNay() : ngay;
}

void DaiLy::hienMenu() {
    std::cout << "\n===== MENU ĐẠI LÝ =====\n"
              << " 1. Tìm chuyến bay\n"
              << " 2. Xem sơ đồ ghế\n"
              << " 3. Đặt vé hộ khách\n"
              << " 4. Xem hoa hồng\n"
              << " 5. Xuất file WEB HTML (datve.html - báo cáo)\n"
              << " 6. Tình trạng ghế/vé từng chuyến (+ xuất file)\n"
              << " 7. Quy định ưu đãi hạng vé & hành lý\n"
              << " 8. *** MỞ WEB ĐẦY ĐỦ (console -> web, 3 vai trò) ***\n"
              << " 0. Đăng xuất\n";
}

std::string DaiLy::vaiTro() const { return "Dai ly"; }


// ----- HanhKhach.cpp -----
HanhKhach::HanhKhach(std::string ma, std::string ten, std::string mk, std::string cccd,
                     std::string sdt, std::string email)
    : NguoiDung(std::move(ma), std::move(ten), std::move(mk)),
      cccd(std::move(cccd)), soDienThoai(std::move(sdt)), email(std::move(email)) {}

std::string HanhKhach::getCCCD() const { return cccd; }
std::string HanhKhach::getSoDienThoai() const { return soDienThoai; }
std::string HanhKhach::getEmail() const { return email; }

void HanhKhach::hienMenu() {
    std::cout << "\n===== MENU HÀNH KHÁCH =====\n"
              << " 1. Tìm chuyến bay\n"
              << " 2. Tìm chuyến nối chuyến\n"
              << " 3. Xem sơ đồ ghế\n"
              << " 4. Đặt vé (giữ chỗ 5 phút)\n"
              << " 5. Đặt vé theo hành trình (nhiều chặng, hành lý, suất ăn)\n"
              << " 6. Check-in chọn chỗ\n"
              << " 7. In boarding pass\n"
              << " 8. Hủy vé\n"
              << " 9. Xuất file WEB HTML (datve.html - báo cáo)\n"
              << "10. *** MỞ WEB ĐẦY ĐỦ (console -> web, 3 vai trò) ***\n"
              << "11. So sánh giá / mua vé qua đại lý\n"
              << " 0. Đăng xuất\n";
}

std::string HanhKhach::vaiTro() const { return "Hanh khach"; }


// ----- SanBay.cpp -----
SanBay::SanBay(std::string ma, std::string ten, std::string thanhPho)
    : maSB(std::move(ma)), ten(std::move(ten)), thanhPho(std::move(thanhPho)) {}

std::string SanBay::getMaSB() const { return maSB; }
std::string SanBay::getTen() const { return ten; }
std::string SanBay::getThanhPho() const { return thanhPho; }


// ----- MayBay.cpp -----
MayBay::MayBay(std::string soHieu, std::string model, int gheTG, int ghePT, int gheTK)
    : soHieu(std::move(soHieu)), model(std::move(model)),
      soGheThuongGia(gheTG), soGhePhoThong(ghePT), soGheTietKiem(gheTK) {}

std::string MayBay::getSoHieu() const { return soHieu; }

int MayBay::getSoGhe(HangGhe h) const {
    switch (h) {
        case HangGhe::THUONG_GIA: return soGheThuongGia;
        case HangGhe::PHO_THONG:  return soGhePhoThong;
        case HangGhe::TIET_KIEM:  return soGheTietKiem;
    }
    return 0;
}

int MayBay::tongSoGhe() const { return soGheThuongGia + soGhePhoThong + soGheTietKiem; }


// ----- HangHangKhong.cpp -----
HangHangKhong::HangHangKhong(std::string ma, std::string ten)
    : maHang(std::move(ma)), ten(std::move(ten)) {}

std::string HangHangKhong::getMaHang() const { return maHang; }
std::string HangHangKhong::getTen() const { return ten; }
void HangHangKhong::themMayBay(MayBay* mb) { if (mb) dsMayBay.push_back(mb); }
void HangHangKhong::themChuyenBay(ChuyenBay* cb) { if (cb) dsChuyenBay.push_back(cb); }
int HangHangKhong::soChuyenBay() const { return static_cast<int>(dsChuyenBay.size()); }


// ----- Ghe.cpp -----
Ghe::Ghe(std::string so, HangGhe h) : soGhe(std::move(so)), hang(h), trangThai(TrangThaiGhe::TRONG) {}

std::string Ghe::getSoGhe() const { return soGhe; }
HangGhe Ghe::getHang() const { return hang; }
TrangThaiGhe Ghe::getTrangThai() const { return trangThai; }
void Ghe::setTrangThai(TrangThaiGhe tt) { trangThai = tt; }


// ----- SoDoGhe.cpp -----
namespace {
std::string tenHang(HangGhe h) {
    switch (h) {
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

// Dat ghe: chap nhan ghe TRONG hoac ghe dang GIU_CHO (xac nhan sau khi giu cho).
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


// ----- GiuCho.cpp -----
GiuCho::GiuCho(ChuyenBay* cb, std::string ghe, HanhKhach* hk, std::chrono::seconds timeout)
    : chuyenBay(cb), soGhe(std::move(ghe)), hanhKhach(hk),
      hetHan(std::chrono::steady_clock::now() + timeout) {}

ChuyenBay* GiuCho::getChuyenBay() const { return chuyenBay; }
std::string GiuCho::getSoGhe() const { return soGhe; }
HanhKhach* GiuCho::getHanhKhach() const { return hanhKhach; }
bool GiuCho::conHieuLuc() const { return std::chrono::steady_clock::now() < hetHan; }


// ----- ChuyenBay.cpp -----
namespace {
const MayBay& kiemTraMayBay(const MayBay* mb) {
    if (!mb) throw LoiNghiepVu("Chuyến bay phai gan voi mot may bay");
    return *mb;
}
}

ChuyenBay::ChuyenBay(std::string ma, SanBay* di, SanBay* den, MayBay* mb, time_point gioDi,
                     time_point gioDen, double giaCoBan)
    : maCB(std::move(ma)), sanBayDi(di), sanBayDen(den), mayBay(mb), soDoGhe(kiemTraMayBay(mb)),
      gioDi(gioDi), gioDen(gioDen), giaCoBan(giaCoBan) {}

std::string ChuyenBay::getMaCB() const { return maCB; }
SanBay* ChuyenBay::getSanBayDi() const { return sanBayDi; }
SanBay* ChuyenBay::getSanBayDen() const { return sanBayDen; }
MayBay* ChuyenBay::getMayBay() const { return mayBay; }
time_point ChuyenBay::getGioDi() const { return gioDi; }
time_point ChuyenBay::getGioDen() const { return gioDen; }
double ChuyenBay::getGiaCoBan() const { return giaCoBan; }
SoDoGhe& ChuyenBay::getSoDoGhe() { return soDoGhe; }

std::chrono::minutes ChuyenBay::thoiGianBay() const {
    return std::chrono::duration_cast<std::chrono::minutes>(gioDen - gioDi);
}

void ChuyenBay::dichGio(std::chrono::seconds delta) {
    gioDi += delta;
    gioDen += delta;
}

void ChuyenBay::datGioBay(time_point gioDiMoi, time_point gioDenMoi) {
    if (gioDenMoi <= gioDiMoi)
        throw LoiNghiepVu("Giờ đến phải sau giờ đi");
    gioDi = gioDiMoi;
    gioDen = gioDenMoi;
}



// Giữ chỗ tam thoi co timeout. Cac phien giu cho (GiuCho) duoc giu den khi chuyen bay bi huy
// de con tro GiuCho* tra ra ngoai khong bao gio bi treo (dangling).
GiuCho* ChuyenBay::giuGhe(std::string soGhe, HanhKhach* hk, std::chrono::seconds timeout) {
    donDepGiuCho();
    if (!soDoGhe.timGhe(soGhe))
        throw LoiHetCho("Ghế " + soGhe + " khong ton tai tren chuyen bay " + maCB);
    if (!soDoGhe.giuGhe(soGhe))
        throw LoiHetCho("Ghế " + soGhe + " da co nguoi dat hoac dang duoc giu");
    dsGiuCho.push_back(std::make_unique<GiuCho>(this, soGhe, hk, timeout));
    return dsGiuCho.back().get();
}

void ChuyenBay::huyGiu(std::string soGhe) { soDoGhe.huyGiu(soGhe); }

// Tra ve trang thai TRONG cho cac ghe co phien giu cho het han (khong co phien nao con hieu luc).
void ChuyenBay::donDepGiuCho() {
    for (const auto& gc : dsGiuCho) {
        if (gc->conHieuLuc()) continue;
        bool conPhienKhacHieuLuc = false;
        for (const auto& khac : dsGiuCho)
            if (khac->getSoGhe() == gc->getSoGhe() && khac->conHieuLuc()) { conPhienKhacHieuLuc = true; break; }
        if (!conPhienKhacHieuLuc) soDoGhe.huyGiu(gc->getSoGhe());
    }
}

void ChuyenBay::datGhe(std::string soGhe) {
    if (!soDoGhe.datGhe(soGhe))
        throw LoiHetCho("Khong the dat ghe " + soGhe + " tren chuyen bay " + maCB);
}

void ChuyenBay::traGhe(std::string soGhe) { soDoGhe.traGhe(soGhe); }

bool ChuyenBay::operator<(const ChuyenBay& o) const { return gioDi < o.gioDi; }

bool ChuyenBay::soSanhTheoGia(const ChuyenBay& a, const ChuyenBay& b) { return a.giaCoBan < b.giaCoBan; }


// ----- HanhTrinh.cpp -----
void HanhTrinh::themChang(ChuyenBay* cb) { if (cb) dsChang.push_back(cb); }
void HanhTrinh::setHanhKhach(HanhKhach* hk) { hanhKhach = hk; }
void HanhTrinh::setHang(HangGhe h) { hang = h; }
void HanhTrinh::setHanhLy(double kg) { soKgHanhLy = kg; }
void HanhTrinh::setSuatAn(std::string mon, double gia) { monAn = std::move(mon); giaSuatAn = gia; }

HanhKhach* HanhTrinh::getHanhKhach() const { return hanhKhach; }
HangGhe HanhTrinh::getHang() const { return hang; }
const std::vector<ChuyenBay*>& HanhTrinh::getDsChang() const { return dsChang; }
double HanhTrinh::getSoKgHanhLy() const { return soKgHanhLy; }
std::string HanhTrinh::getMonAn() const { return monAn; }
double HanhTrinh::getGiaSuatAn() const { return giaSuatAn; }
int HanhTrinh::soChang() const { return static_cast<int>(dsChang.size()); }

std::chrono::minutes HanhTrinh::tongThoiGian() const {
    if (dsChang.empty()) return std::chrono::minutes(0);
    return std::chrono::duration_cast<std::chrono::minutes>(dsChang.back()->getGioDen() - dsChang.front()->getGioDi());
}


// ----- VeMayBay.cpp -----
namespace {
std::string dinhDangGio(time_point tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[32];
    std::strftime(buf, sizeof buf, "%d/%m/%Y %H:%M", &tm);
    return buf;
}
}

VeMayBay::VeMayBay(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : maVe(std::move(ma)), soGhe(std::move(ghe)), chuyenBay(cb), hanhKhach(hk),
      ngayDat(std::chrono::system_clock::now()), daCheckIn(false) {}

VeMayBay::~VeMayBay() = default;

std::string VeMayBay::getMaVe() const { return maVe; }
std::string VeMayBay::getSoGhe() const { return soGhe; }
void VeMayBay::setSoGhe(std::string ghe) { soGhe = std::move(ghe); }
ChuyenBay* VeMayBay::getChuyenBay() const { return chuyenBay; }
HanhKhach* VeMayBay::getHanhKhach() const { return hanhKhach; }
bool VeMayBay::isDaCheckIn() const { return daCheckIn; }
void VeMayBay::danhDauCheckIn() { daCheckIn = true; }

std::string VeMayBay::moTa() const { return maVe + " - " + loaiVe(); }

const VeMayBay* VeMayBay::veGoc() const { return this; }

void VeMayBay::inBoardingPass() const {
    const std::string ten = hanhKhach ? hanhKhach->getHoTen() : "?";
    std::cout << "\n+--------------------------------------------------+\n"
              << "|                  BOARDING PASS                   |\n"
              << "+--------------------------------------------------+\n"
              << " Hanh khach : " << ten << "\n"
              << " Ma ve      : " << maVe << "\n"
              << " Hang ve    : " << loaiVe() << "\n";
    if (chuyenBay) {
        const SanBay* di = chuyenBay->getSanBayDi();
        const SanBay* den = chuyenBay->getSanBayDen();
        std::cout << " Chuyến bay : " << chuyenBay->getMaCB() << "\n"
                  << " Hanh trinh : " << (di ? di->getMaSB() : "?") << " -> " << (den ? den->getMaSB() : "?") << "\n"
                  << " Giờ đi     : " << dinhDangGio(chuyenBay->getGioDi()) << "\n"
                  << " Giờ đến    : " << dinhDangGio(chuyenBay->getGioDen()) << "\n";
    }
    std::cout << " So ghe     : " << soGhe << "\n";
    if (const VeThuongGia* tg = dynamic_cast<const VeThuongGia*>(veGoc()))
        std::cout << " Phong cho  : " << (tg->coPhongCho() ? "Phong cho thuong gia" : "Khong") << "\n";
    std::cout << " Chi tiet   : " << moTa() << "\n"
              << "+--------------------------------------------------+\n";
}


// ----- VeThuongGia.cpp -----
VeThuongGia::VeThuongGia(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : VeMayBay(std::move(ma), std::move(ghe), cb, hk) {}

double VeThuongGia::tinhGia() const { return chuyenBay->getGiaCoBan() * HE_SO; }

std::string VeThuongGia::loaiVe() const { return "Thương gia"; }

bool VeThuongGia::coPhongCho() const { return true; }


// ----- VePhoThong.cpp -----
VePhoThong::VePhoThong(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : VeMayBay(std::move(ma), std::move(ghe), cb, hk) {}

double VePhoThong::tinhGia() const { return chuyenBay->getGiaCoBan() * HE_SO; }

std::string VePhoThong::loaiVe() const { return "Phổ thông"; }


// ----- VeTietKiem.cpp -----
VeTietKiem::VeTietKiem(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : VeMayBay(std::move(ma), std::move(ghe), cb, hk) {}

double VeTietKiem::tinhGia() const { return chuyenBay->getGiaCoBan() * HE_SO; }

std::string VeTietKiem::loaiVe() const { return "Tiết kiệm"; }

bool VeTietKiem::choPhepDoiVe() const { return false; }


// ----- VeDecorator.cpp -----
namespace {
const VeMayBay& kiemTraVe(const std::unique_ptr<VeMayBay>& v) {
    if (!v) throw LoiNghiepVu("Decorator can mot ve goc hop le");
    return *v;
}
}

// Decorator vua IS-A (ke thua VeMayBay) vua HAS-A (chua ve goc trong "ve").
VeDecorator::VeDecorator(std::unique_ptr<VeMayBay> v)
    : VeMayBay(kiemTraVe(v).getMaVe(), v->getSoGhe(), v->getChuyenBay(), v->getHanhKhach()),
      ve(std::move(v)) {
    if (ve->isDaCheckIn()) danhDauCheckIn();
}

const VeMayBay* VeDecorator::veGoc() const { return ve->veGoc(); }

std::string VeDecorator::loaiVe() const { return ve->loaiVe(); }


// ----- DichVuGiaTang.cpp -----
DichVuGiaTang::DichVuGiaTang(std::string ma, std::string ten) : maDV(std::move(ma)), ten(std::move(ten)) {}

DichVuGiaTang::~DichVuGiaTang() = default;


// ----- HanhLy.cpp -----
HanhLy::HanhLy(double kg, double donGiaKg)
    : DichVuGiaTang("HL", "Hanh ly"), soKg(kg), donGiaKg(donGiaKg) {}

double HanhLy::tinhPhi() const { return soKg * donGiaKg; }

// Dinh dang co dinh "Hanh ly <kg>kg (<don gia>/kg)": FileRepository doc lai chuoi nay khi nap ve tu file.
std::string HanhLy::moTa() const {
    std::ostringstream os;
    os << std::setprecision(10) << "Hanh ly " << soKg << "kg (" << donGiaKg << "/kg)";
    return os.str();
}


// ----- VeCoHanhLy.cpp -----
VeCoHanhLy::VeCoHanhLy(std::unique_ptr<VeMayBay> v, HanhLy hl)
    : VeDecorator(std::move(v)), hanhLy(hl) {}

double VeCoHanhLy::tinhGia() const { return ve->tinhGia() + hanhLy.tinhPhi(); }

std::string VeCoHanhLy::moTa() const { return ve->moTa() + " + " + hanhLy.moTa(); }


// ----- SuatAn.cpp -----
SuatAn::SuatAn(std::string mon, double gia) : DichVuGiaTang("SA", "Suat an"), monAn(std::move(mon)), gia(gia) {}

double SuatAn::tinhPhi() const { return gia; }

// Dinh dang co dinh "Suat an: <mon> (<gia>)": FileRepository doc lai chuoi nay khi nap ve tu file.
std::string SuatAn::moTa() const {
    std::ostringstream os;
    os << std::setprecision(10) << "Suat an: " << monAn << " (" << gia << ")";
    return os.str();
}


// ----- VeCoSuatAn.cpp -----
VeCoSuatAn::VeCoSuatAn(std::unique_ptr<VeMayBay> v, SuatAn sa)
    : VeDecorator(std::move(v)), suatAn(sa) {}

double VeCoSuatAn::tinhGia() const { return ve->tinhGia() + suatAn.tinhPhi(); }

std::string VeCoSuatAn::moTa() const { return ve->moTa() + " + " + suatAn.moTa(); }


// ----- VeFactory.cpp -----
std::unique_ptr<VeMayBay> VeFactory::taoVe(HangGhe h, std::string maVe, std::string soGhe, ChuyenBay* cb,
                                           HanhKhach* hk) {
    switch (h) {
        case HangGhe::THUONG_GIA: return std::make_unique<VeThuongGia>(std::move(maVe), std::move(soGhe), cb, hk);
        case HangGhe::PHO_THONG:  return std::make_unique<VePhoThong>(std::move(maVe), std::move(soGhe), cb, hk);
        case HangGhe::TIET_KIEM:  return std::make_unique<VeTietKiem>(std::move(maVe), std::move(soGhe), cb, hk);
    }
    throw LoiNghiepVu("Hang ghe khong hop le");
}


// ----- HanhTrinhBuilder.cpp -----
HanhTrinhBuilder::HanhTrinhBuilder() {}

HanhTrinhBuilder& HanhTrinhBuilder::themChang(ChuyenBay* cb) { hanhTrinh.themChang(cb); return *this; }
HanhTrinhBuilder& HanhTrinhBuilder::chonHang(HangGhe h) { hanhTrinh.setHang(h); return *this; }
HanhTrinhBuilder& HanhTrinhBuilder::chonHanhKhach(HanhKhach* hk) { hanhTrinh.setHanhKhach(hk); return *this; }
HanhTrinhBuilder& HanhTrinhBuilder::themHanhLy(double kg) { hanhTrinh.setHanhLy(kg); return *this; }
HanhTrinhBuilder& HanhTrinhBuilder::themSuatAn(std::string mon, double gia) {
    hanhTrinh.setSuatAn(std::move(mon), gia);
    return *this;
}

HanhTrinh HanhTrinhBuilder::build() { return hanhTrinh; }


// ----- QuanLyChuyenBay.cpp -----
namespace {
// San bay "khop" khi chuoi rong (khong loc), hoac trung ma san bay, hoac trung ten thanh pho.
bool khopSanBay(const SanBay* sb, const std::string& s) {
    return s.empty() || (sb && (sb->getMaSB() == s || sb->getThanhPho() == s));
}

bool cungNgay(time_point a, time_point b) {
    std::time_t ta = std::chrono::system_clock::to_time_t(a);
    std::time_t tb = std::chrono::system_clock::to_time_t(b);
    std::tm x{}, y{};
#ifdef _WIN32
    localtime_s(&x, &ta);
    localtime_s(&y, &tb);
#else
    localtime_r(&ta, &x);
    localtime_r(&tb, &y);
#endif
    return x.tm_year == y.tm_year && x.tm_yday == y.tm_yday;
}

// time_point{} (moc epoch) nghia la "khong loc theo ngay".
bool khopNgay(const ChuyenBay* cb, time_point ngay) {
    return ngay == time_point{} || cungNgay(cb->getGioDi(), ngay);
}
}

void QuanLyChuyenBay::themChuyenBay(std::unique_ptr<ChuyenBay> cb) {
    if (!cb) throw LoiNghiepVu("Chuyến bay không hợp lệ");
    if (timChuyenBay(cb->getMaCB()))
        throw LoiNghiepVu("Trùng mã chuyến bay " + cb->getMaCB());
    // Cùng số hiệu máy bay không được trùng/giao khung giờ bay
    MayBay* mb = cb->getMayBay();
    if (mb) {
        const auto di = cb->getGioDi();
        const auto den = cb->getGioDen();
        if (den <= di)
            throw LoiNghiepVu("Giờ đến phải sau giờ đi");
        for (const auto& x : dsChuyenBay) {
            if (!x || !x->getMayBay()) continue;
            if (x->getMayBay()->getSoHieu() != mb->getSoHieu()) continue;
            // Giao nhau: di < x.den && den > x.di
            if (di < x->getGioDen() && den > x->getGioDi()) {
                throw LoiNghiepVu(
                    "Trùng lịch máy bay " + mb->getSoHieu() +
                    " với chuyến " + x->getMaCB() +
                    " (cùng tàu bay, khung giờ giao nhau)");
            }
        }
    }
    dsChuyenBay.push_back(std::move(cb));
}

// Khong cho xoa chuyen bay da co ve / dang giu cho (tranh con tro treo o ve va giu cho).
void QuanLyChuyenBay::xoaChuyenBay(std::string maCB) {
    auto it = std::find_if(dsChuyenBay.begin(), dsChuyenBay.end(),
                           [&](const std::unique_ptr<ChuyenBay>& c) { return c->getMaCB() == maCB; });
    if (it == dsChuyenBay.end()) throw LoiChuyenBayKhongTonTai("Khong co chuyen bay " + maCB);
    ChuyenBay* cb = it->get();
    cb->donDepGiuCho();
    int conTrong = 0;
    for (HangGhe h : {HangGhe::THUONG_GIA, HangGhe::PHO_THONG, HangGhe::TIET_KIEM})
        conTrong += cb->getSoDoGhe().demGheTrong(h);
    if (conTrong != cb->getMayBay()->tongSoGhe())
        throw LoiNghiepVu("Chuyến bay " + maCB + " da co ve hoac dang giu cho, khong the xoa");
    dsChuyenBay.erase(it);
}

void QuanLyChuyenBay::themSanBay(std::unique_ptr<SanBay> sb) {
    if (!sb) throw LoiNghiepVu("San bay khong hop le");
    if (timSanBay(sb->getMaSB())) throw LoiNghiepVu("Trung ma san bay " + sb->getMaSB());
    dsSanBay.push_back(std::move(sb));
}

void QuanLyChuyenBay::themMayBay(std::unique_ptr<MayBay> mb) {
    if (!mb) throw LoiNghiepVu("May bay khong hop le");
    if (timMayBay(mb->getSoHieu())) throw LoiNghiepVu("Trung so hieu may bay " + mb->getSoHieu());
    dsMayBay.push_back(std::move(mb));
}

void QuanLyChuyenBay::themHang(std::unique_ptr<HangHangKhong> h) {
    if (!h) throw LoiNghiepVu("Hang hang khong khong hop le");
    for (const auto& x : dsHang)
        if (x->getMaHang() == h->getMaHang()) throw LoiNghiepVu("Trung ma hang " + h->getMaHang());
    dsHang.push_back(std::move(h));
}

ChuyenBay* QuanLyChuyenBay::timChuyenBay(std::string maCB) const {
    for (const auto& cb : dsChuyenBay) if (cb->getMaCB() == maCB) return cb.get();
    return nullptr;
}

SanBay* QuanLyChuyenBay::timSanBay(std::string maSB) const {
    for (const auto& sb : dsSanBay) if (sb->getMaSB() == maSB) return sb.get();
    return nullptr;
}

MayBay* QuanLyChuyenBay::timMayBay(std::string soHieu) const {
    for (const auto& mb : dsMayBay) if (mb->getSoHieu() == soHieu) return mb.get();
    return nullptr;
}

// di / den: ma san bay hoac thanh pho ("" = bat ky). ngay: time_point{} = bat ky ngay.
std::vector<ChuyenBay*> QuanLyChuyenBay::timChuyenBay(std::string di, std::string den, time_point ngay) const {
    std::vector<ChuyenBay*> kq;
    for (const auto& cb : dsChuyenBay)
        if (khopSanBay(cb->getSanBayDi(), di) && khopSanBay(cb->getSanBayDen(), den) && khopNgay(cb.get(), ngay))
            kq.push_back(cb.get());
    return kq;
}

// Tim hanh trinh noi chuyen (tu 2 den toiDaChang chang): chang sau khoi hanh sau khi chang truoc
// den it nhat choToiThieu, khong di lai san bay da qua. Ket qua sap theo gio den cuoi tang dan.
std::vector<std::vector<ChuyenBay*> > QuanLyChuyenBay::timNoiChuyen(std::string di, std::string den, time_point ngay,
                                                                      std::chrono::minutes choToiThieu,
                                                                      int toiDaChang) const {
    std::vector<std::vector<ChuyenBay*> > kq;
    std::vector<ChuyenBay*> duong;
    std::function<void()> duyet = [&]() {
        ChuyenBay* cuoi = duong.back();
        if (duong.size() >= 2 && khopSanBay(cuoi->getSanBayDen(), den)) kq.push_back(duong);
        if (static_cast<int>(duong.size()) >= toiDaChang) return;
        for (const auto& cb : dsChuyenBay) {
            ChuyenBay* tiep = cb.get();
            if (tiep->getSanBayDi() != cuoi->getSanBayDen()) continue;
            if (tiep->getGioDi() < cuoi->getGioDen() + choToiThieu) continue;
            bool quayLai = false;
            for (const ChuyenBay* c : duong)
                if (c->getSanBayDi() == tiep->getSanBayDen()) { quayLai = true; break; }
            if (quayLai) continue;
            duong.push_back(tiep);
            duyet();
            duong.pop_back();
        }
    };
    for (const auto& cb : dsChuyenBay) {
        if (!khopSanBay(cb->getSanBayDi(), di) || !khopNgay(cb.get(), ngay)) continue;
        duong.assign(1, cb.get());
        duyet();
    }
    std::sort(kq.begin(), kq.end(), [](const std::vector<ChuyenBay*>& a, const std::vector<ChuyenBay*>& b) {
        return a.back()->getGioDen() < b.back()->getGioDen();
    });
    return kq;
}

void QuanLyChuyenBay::sapXepTheoGio(std::vector<ChuyenBay*>& ds) {
    std::sort(ds.begin(), ds.end(), [](const ChuyenBay* a, const ChuyenBay* b) { return *a < *b; });
}

void QuanLyChuyenBay::sapXepTheoGia(std::vector<ChuyenBay*>& ds) {
    std::sort(ds.begin(), ds.end(),
              [](const ChuyenBay* a, const ChuyenBay* b) { return ChuyenBay::soSanhTheoGia(*a, *b); });
}

std::vector<SanBay*> QuanLyChuyenBay::layDsSanBay() const {
    std::vector<SanBay*> kq;
    for (const auto& x : dsSanBay) kq.push_back(x.get());
    return kq;
}
std::vector<MayBay*> QuanLyChuyenBay::layDsMayBay() const {
    std::vector<MayBay*> kq;
    for (const auto& x : dsMayBay) kq.push_back(x.get());
    return kq;
}
std::vector<ChuyenBay*> QuanLyChuyenBay::layDsChuyenBay() const {
    std::vector<ChuyenBay*> kq;
    for (const auto& x : dsChuyenBay) kq.push_back(x.get());
    return kq;
}
std::vector<HangHangKhong*> QuanLyChuyenBay::layDsHang() const {
    std::vector<HangHangKhong*> kq;
    for (const auto& x : dsHang) kq.push_back(x.get());
    return kq;
}


// ----- QuanLyDatVe.cpp -----
namespace {
const std::chrono::seconds THOI_GIAN_GIU_CHO{300};   // giu cho tam thoi 5 phut
const std::chrono::hours MO_CHECK_IN{24};            // mo check-in truoc gio bay 24 gio
const std::chrono::hours DONG_CHECK_IN{1};           // dong check-in truoc gio bay 1 gio
const double DON_GIA_HANH_LY = 50000.0;              // phi hanh ly moi kg

// Ma ve = "VE" + so thu tu lon nhat dang co + 1 (dung duoc ca sau khi nap ve tu file).
std::string taoMaVe(const std::vector<std::unique_ptr<VeMayBay> >& ds) {
    long lonNhat = 0;
    for (const auto& v : ds) {
        const std::string ma = v->getMaVe();
        if (ma.size() > 2 && ma.compare(0, 2, "VE") == 0) {
            try { lonNhat = std::max(lonNhat, std::stol(ma.substr(2))); } catch (...) {}
        }
    }
    char buf[24];
    std::snprintf(buf, sizeof buf, "VE%05ld", lonNhat + 1);
    return buf;
}
}

QuanLyDatVe::QuanLyDatVe(QuanLyChuyenBay& ql) : qlChuyenBay(ql) {}

GiuCho* QuanLyDatVe::giuCho(std::string maCB, std::string soGhe, HanhKhach* hk) {
    if (!hk) throw LoiNghiepVu("Thieu thong tin hanh khach");
    ChuyenBay* cb = qlChuyenBay.timChuyenBay(maCB);
    if (!cb) throw LoiChuyenBayKhongTonTai("Khong co chuyen bay " + maCB);
    return cb->giuGhe(soGhe, hk, THOI_GIAN_GIU_CHO);
}

VeMayBay* QuanLyDatVe::xacNhanThanhToan(GiuCho* gc, HangGhe h, DaiLy* dl) {
    if (!gc) throw LoiGiuChoHetHan("Khong co phien giu cho");
    ChuyenBay* cb = gc->getChuyenBay();
    const std::string soGhe = gc->getSoGhe();
    HanhKhach* hk = gc->getHanhKhach();
    if (!gc->conHieuLuc()) {
        cb->donDepGiuCho();
        throw LoiGiuChoHetHan("Giữ chỗ ghe " + soGhe + " da het han, vui long dat lai");
    }
    Ghe* ghe = cb->getSoDoGhe().timGhe(soGhe);
    if (!ghe || ghe->getTrangThai() != TrangThaiGhe::GIU_CHO)
        throw LoiGiuChoHetHan("Phien giu cho ghe " + soGhe + " khong con hieu luc");
    if (ghe->getHang() != h) {
        cb->huyGiu(soGhe);
        throw LoiNghiepVu("Ghế " + soGhe + " khong thuoc hang ve da chon");
    }
    std::unique_ptr<VeMayBay> ve = VeFactory::taoVe(h, taoMaVe(dsVe), soGhe, cb, hk);
    VeMayBay* raw = ve.get();
    dsVe.push_back(std::move(ve));
    cb->datGhe(soGhe);
    if (dl) dl->congHoaHong(raw->tinhGia());
    return raw;
}

std::vector<VeMayBay*> QuanLyDatVe::datVeTheoHanhTrinh(const HanhTrinh& ht, DaiLy* dl) {
    if (!ht.getHanhKhach() || ht.soChang() == 0) throw LoiNghiepVu("Hanh trinh khong hop le");
    // Buoc 1: giu cho tat ca cac chang; neu mot chang that bai thi tra lai cac ghe da giu.
    std::vector<GiuCho*> dsGc;
    try {
        for (ChuyenBay* cb : ht.getDsChang()) {
            cb->donDepGiuCho();
            std::vector<Ghe*> trong = cb->getSoDoGhe().layGheTrong(ht.getHang());
            if (trong.empty()) throw LoiHetCho("Chuyến bay " + cb->getMaCB() + " het ghe hang nay");
            dsGc.push_back(cb->giuGhe(trong.front()->getSoGhe(), ht.getHanhKhach(), THOI_GIAN_GIU_CHO));
        }
    } catch (...) {
        for (GiuCho* gc : dsGc) gc->getChuyenBay()->huyGiu(gc->getSoGhe());
        throw;
    }
    // Buoc 2: moi chang tao 1 ve bang Factory, co hanh ly / suat an thi boc them Decorator.
    std::vector<VeMayBay*> ketQua;
    for (GiuCho* gc : dsGc) {
        VeMayBay* ve = xacNhanThanhToan(gc, ht.getHang(), nullptr);
        auto it = std::find_if(dsVe.begin(), dsVe.end(),
                               [&](const std::unique_ptr<VeMayBay>& p) { return p.get() == ve; });
        if (ht.getSoKgHanhLy() > 0)
            *it = std::make_unique<VeCoHanhLy>(std::move(*it), HanhLy(ht.getSoKgHanhLy(), DON_GIA_HANH_LY));
        if (!ht.getMonAn().empty())
            *it = std::make_unique<VeCoSuatAn>(std::move(*it), SuatAn(ht.getMonAn(), ht.getGiaSuatAn()));
        ve = it->get();
        if (dl) dl->congHoaHong(ve->tinhGia());
        ketQua.push_back(ve);
    }
    return ketQua;
}

void QuanLyDatVe::huyVe(std::string maVe) {
    auto it = std::find_if(dsVe.begin(), dsVe.end(),
                           [&](const std::unique_ptr<VeMayBay>& v) { return v->getMaVe() == maVe; });
    if (it == dsVe.end()) throw LoiNghiepVu("Khong tim thay ve " + maVe);
    (*it)->getChuyenBay()->traGhe((*it)->getSoGhe());
    dsVe.erase(it);
}

void QuanLyDatVe::checkIn(std::string maVe, std::string soGheMoi) {
    VeMayBay* ve = timVe(maVe);
    if (!ve) throw LoiNghiepVu("Khong tim thay ve " + maVe);
    if (ve->isDaCheckIn()) throw LoiNghiepVu("Ve " + maVe + " da check-in roi");
    ChuyenBay* cb = ve->getChuyenBay();
    const auto bay = std::chrono::system_clock::now();
    if (bay < cb->getGioDi() - MO_CHECK_IN) throw LoiNghiepVu("Chua den gio mo check-in (mo truoc gio bay 24 gio)");
    if (bay > cb->getGioDi() - DONG_CHECK_IN) throw LoiNghiepVu("Da dong check-in (dong truoc gio bay 1 gio)");

    const std::string soGheCu = ve->getSoGhe();
    if (soGheMoi != soGheCu) {
        const VeTietKiem* tk = dynamic_cast<const VeTietKiem*>(ve->veGoc());
        if (tk && !tk->choPhepDoiVe()) throw LoiNghiepVu("Ve tiet kiem khong duoc doi cho ngoi");
        cb->donDepGiuCho();
        SoDoGhe& sd = cb->getSoDoGhe();
        Ghe* gheCu = sd.timGhe(soGheCu);
        Ghe* gheMoi = sd.timGhe(soGheMoi);
        if (!gheMoi) throw LoiNghiepVu("Ghế " + soGheMoi + " khong ton tai");
        if (gheCu && gheMoi->getHang() != gheCu->getHang())
            throw LoiNghiepVu("Chi duoc doi sang ghe cung hang ve");
        if (gheMoi->getTrangThai() != TrangThaiGhe::TRONG || !sd.datGhe(soGheMoi))
            throw LoiHetCho("Ghế " + soGheMoi + " da co nguoi dat hoac dang duoc giu");
        cb->traGhe(soGheCu);
        ve->setSoGhe(soGheMoi);
    }
    ve->danhDauCheckIn();
}

void QuanLyDatVe::inBoardingPass(std::string maVe) const {
    VeMayBay* ve = timVe(maVe);
    if (!ve) throw LoiNghiepVu("Khong tim thay ve " + maVe);
    if (!ve->isDaCheckIn()) throw LoiNghiepVu("Ve " + maVe + " chua check-in, khong the in boarding pass");
    ve->inBoardingPass();
}

VeMayBay* QuanLyDatVe::timVe(std::string maVe) const {
    for (const auto& v : dsVe) if (v->getMaVe() == maVe) return v.get();
    return nullptr;
}

// Phan loai theo ve goc (ke ca ve da boc Decorator) bang dynamic_cast.
std::map<std::string, int> QuanLyDatVe::thongKeLoaiVe() const {
    std::map<std::string, int> kq{{"Thương gia", 0}, {"Phổ thông", 0}, {"Tiết kiệm", 0}};
    for (const auto& v : dsVe) {
        const VeMayBay* goc = v->veGoc();
        if (dynamic_cast<const VeThuongGia*>(goc)) ++kq["Thương gia"];
        else if (dynamic_cast<const VePhoThong*>(goc)) ++kq["Phổ thông"];
        else if (dynamic_cast<const VeTietKiem*>(goc)) ++kq["Tiết kiệm"];
    }
    return kq;
}

double QuanLyDatVe::tinhDoanhThu() const {
    double tong = 0;
    for (const auto& v : dsVe) tong += v->tinhGia();
    return tong;
}

std::vector<VeMayBay*> QuanLyDatVe::layDsVe() const {
    std::vector<VeMayBay*> kq;
    for (const auto& v : dsVe) kq.push_back(v.get());
    return kq;
}


// ----- QuanLyNguoiDung.cpp -----
void QuanLyNguoiDung::themNguoiDung(std::unique_ptr<NguoiDung> nd) {
    if (!nd) throw LoiNghiepVu("Nguoi dung khong hop le");
    if (timNguoiDung(nd->getMaND())) throw LoiNghiepVu("Trung ma nguoi dung " + nd->getMaND());
    dsNguoiDung.push_back(std::move(nd));
}

NguoiDung* QuanLyNguoiDung::dangNhap(std::string ma, std::string mk) const {
    NguoiDung* nd = timNguoiDung(ma);
    return (nd && nd->dangNhap(mk)) ? nd : nullptr;
}

NguoiDung* QuanLyNguoiDung::timNguoiDung(std::string ma) const {
    for (const auto& nd : dsNguoiDung) if (nd->getMaND() == ma) return nd.get();
    return nullptr;
}

std::vector<NguoiDung*> QuanLyNguoiDung::layDsNguoiDung() const {
    std::vector<NguoiDung*> kq;
    for (const auto& nd : dsNguoiDung) kq.push_back(nd.get());
    return kq;
}


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


// ======================================================================
// PHAN 4: DU LIEU MAU + HAM MAIN
// ======================================================================

namespace {
// Lan chay dau (chua co thu muc data/): tao du lieu mau bang chinh cac ham public cua UML.

// Dich toan bo chuyen bay de gan voi "bay gio" (giu nguyen khoang cach giua cac chuyen).
// Goi moi lan khoi dong / mo web de gio bay luon theo ngay thuc te.
void capNhatGioChuyenTheoThoiGianThuc(QuanLyChuyenBay& ql) {
    // Lịch bay NEO THEO ĐỒNG HỒ MÁY + khung giờ nội địa VN (mẫu sát thực tế):
    // - Thời lượng theo tuyến (SGN-HAN ~2h05, SGN-DAD ~1h15, ...)
    // - Nhiều suất trong ngày (sáng / trưa / tối) như lịch hãng
    // - Hôm nay còn slot → hiện hôm nay; slot đã qua → ngày mai
    // Không gọi API hãng (BTL không database); giờ luôn bám ngày/giờ hệ thống.
    auto ds = ql.layDsChuyenBay();
    if (ds.empty()) return;

    using clock = std::chrono::system_clock;
    const auto now = clock::now();
    const std::time_t now_tt = clock::to_time_t(now);

    auto phutBay = [](const std::string& a, const std::string& b) -> int {
        auto eq = [&](const char* x, const char* y) {
            return (a == x && b == y) || (a == y && b == x);
        };
        if (eq("SGN", "HAN")) return 125;
        if (eq("SGN", "DAD") || eq("HAN", "DAD")) return 75;
        if (eq("SGN", "PQC")) return 60;
        if (eq("SGN", "CXR")) return 55;
        if (eq("SGN", "VCA") || eq("SGN", "DLI")) return 50;
        if (eq("SGN", "HUI") || eq("HAN", "HUI")) return 75;
        if (eq("HAN", "HPH")) return 35;
        if (eq("HAN", "VII") || eq("HAN", "VDO")) return 45;
        if (eq("SGN", "UIH")) return 70;
        if (eq("DAD", "SGN") || eq("DAD", "HAN")) return 75;
        return 90;
    };

    // Suất bay trong ngày (giờ địa phương) — mật độ gần lịch nội địa
    static const int SLOT[][2] = {
        {5, 30}, {6, 0}, {6, 30}, {7, 0}, {7, 30}, {8, 0}, {8, 30}, {9, 15},
        {10, 0}, {10, 45}, {11, 30}, {12, 15}, {13, 0}, {14, 0}, {15, 0},
        {16, 0}, {17, 0}, {17, 45}, {18, 30}, {19, 15}, {20, 0}, {20, 45},
        {21, 30}, {22, 15}
    };
    const int NS = 24;

    // Ổn định slot theo mã chuyến (cùng mã = cùng giờ mỗi ngày)
    auto slotChoMa = [&](const std::string& ma) -> int {
        unsigned h = 2166136261u;
        for (unsigned char c : ma) { h ^= c; h *= 16777619u; }
        return static_cast<int>(h % static_cast<unsigned>(NS));
    };

    int ganHomNay = 0, ganHomMai = 0;
    for (ChuyenBay* cb : ds) {
        if (!cb || !cb->getSanBayDi() || !cb->getSanBayDen()) continue;
        const std::string di = cb->getSanBayDi()->getMaSB();
        const std::string den = cb->getSanBayDen()->getMaSB();
        const int dur = phutBay(di, den);
        const int s = slotChoMa(cb->getMaCB());
        int gio = SLOT[s][0];
        int phut = SLOT[s][1];

        std::tm tmMoc{};
#ifdef _WIN32
        localtime_s(&tmMoc, &now_tt);
#else
        localtime_r(&now_tt, &tmMoc);
#endif
        tmMoc.tm_hour = gio;
        tmMoc.tm_min = phut;
        tmMoc.tm_sec = 0;
        std::time_t tDi = std::mktime(&tmMoc);
        auto gioDi = clock::from_time_t(tDi);

        // Đã qua giờ khởi hành (cho phép trễ 20 phút vẫn coi là chuyến hôm nay sắp/tới)
        if (gioDi + std::chrono::minutes(20) < now) {
            tDi += 24 * 60 * 60;
            gioDi = clock::from_time_t(tDi);
            ++ganHomMai;
        } else {
            ++ganHomNay;
        }
        const auto gioDen = gioDi + std::chrono::minutes(dur);
        try {
            cb->datGioBay(gioDi, gioDen);
        } catch (...) {
            // fallback dichGio
            const auto delta = std::chrono::duration_cast<std::chrono::seconds>(gioDi - cb->getGioDi());
            cb->dichGio(delta);
        }
    }
    std::cout << "Đã cập nhật giờ bay theo ngày/giờ máy: "
              << ganHomNay << " chuyến hôm nay, "
              << ganHomMai << " chuyến ngày mai"
              << " (khung giờ nội địa + thời lượng theo tuyến).\n";
}






void soSanhGiaMuaQuaDaiLy(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, HanhKhach* hk) {
    std::cout << "\n=== SO SÁNH GIÁ: QUẦY TRỰC TIẾP vs ĐẠI LÝ ===\n";
    std::vector<ChuyenBay*> ds = ql.layDsChuyenBay();
    ql.sapXepTheoGio(ds);
    ChuyenBay* cb = chonChuyenBayTuDs(ds, "Chọn chuyến bay để so sánh giá");
    if (!cb) return;
    const HangGhe hang = chonHang();
    const double giaGoc = cb->getGiaCoBan();
    double heso = 1.0;
    if (hang == HangGhe::THUONG_GIA) heso = 2.5;
    else if (hang == HangGhe::TIET_KIEM) heso = 0.8;
    const double giaQuay = giaGoc * heso;
    std::cout << "\n  Chuyến " << cb->getMaCB() << " | " << cb->getSanBayDi()->getMaSB()
              << "→" << cb->getSanBayDen()->getMaSB() << "\n";
    std::cout << "  Giá cơ bản: " << dinhDangTien(giaGoc) << " x hệ số hạng = "
              << dinhDangTien(giaQuay) << " (mua trực tiếp tại quầy / hệ thống)\n\n";
    std::cout << "  STT  Đại lý                      Tỷ lệ HH   Giá ước tính(*)   Ghi chú\n";
    std::cout << "  -----------------------------------------------------------------------------\n";
    std::cout << "   0   Mua trực tiếp (quầy)         0%        " << dinhDangTien(giaQuay) << "   Không qua đại lý\n";
    int stt = 0;
    struct HangDL { DaiLy* dl; double gia; };
    std::vector<HangDL> bang;
    for (NguoiDung* u : nd.layDsNguoiDung()) {
        auto* dl = dynamic_cast<DaiLy*>(u);
        if (!dl) continue;
        // Dai ly thuong ban gan gia quay, chiet khau nhe theo ty le (gia = quay * (1 - tyLe*0.5))
        const double giaDL = giaQuay * (1.0 - dl->getTyLeHoaHong() * 0.5);
        bang.push_back({dl, giaDL});
    }
    std::sort(bang.begin(), bang.end(), [](const HangDL& a, const HangDL& b) { return a.gia < b.gia; });
    for (const auto& row : bang) {
        ++stt;
        std::cout << "  " << std::setw(3) << stt << "  "
                  << std::left << std::setw(28) << row.dl->getHoTen()
                  << std::right << std::setw(6) << int(row.dl->getTyLeHoaHong()*100) << "%   "
                  << dinhDangTien(row.gia);
        if (stt == 1) std::cout << "   << rẻ nhất";
        std::cout << "\n";
    }
    std::cout << "  (*) Giá ước tính để so sánh — đặt thật qua đại lý dùng menu Đặt vé (đăng nhập DL).\n";
    std::cout << "  Bạn có thể mua trực tiếp: chọn menu 4 hoặc 5 (Đặt vé).\n";
}

void xuatMaTranNoiChuyen(QuanLyChuyenBay& ql, const std::string& path) {
    auto dssb = ql.layDsSanBay();
    std::ofstream f(path);
    if (!f) { std::cout << "Khong ghi duoc " << path << "\n"; return; }
    f << "================================================================================\n";
    f << "  MA TRAN NOI CHUYEN GIUA CAC SAN BAY (tu dong sinh)\n";
    f << "  Moi cap A -> B: liet ke bay thang va/hoac noi chuyen (doi canh)\n";
    f << "================================================================================\n\n";
    int coNoi = 0, coThang = 0;
    for (SanBay* a : dssb) {
        for (SanBay* b : dssb) {
            if (a == b) continue;
            auto thang = ql.timChuyenBay(a->getMaSB(), b->getMaSB(), time_point{});
            auto noi = ql.timNoiChuyen(a->getMaSB(), b->getMaSB(), time_point{}, std::chrono::minutes(45), 3);
            // loc noi chi nhieu hon 1 chang
            std::vector<std::vector<ChuyenBay*>> noi2;
            for (auto& d : noi) if (d.size() >= 2) noi2.push_back(d);
            if (thang.empty() && noi2.empty()) continue;
            f << ">>> " << a->getMaSB() << " (" << a->getThanhPho() << ") -> "
              << b->getMaSB() << " (" << b->getThanhPho() << ")\n";
            if (!thang.empty()) {
                ++coThang;
                f << "  BAY THANG (" << thang.size() << "):\n";
                for (ChuyenBay* cb : thang)
                    f << "    - " << cb->getMaCB() << "\n";
            } else {
                f << "  BAY THANG: khong co\n";
            }
            if (!noi2.empty()) {
                ++coNoi;
                f << "  NOI CHUYEN (" << noi2.size() << " PA):\n";
                for (auto& d : noi2) {
                    f << "    - ";
                    f << d.front()->getSanBayDi()->getMaSB();
                    for (size_t k = 0; k < d.size(); ++k) {
                        f << " -> " << d[k]->getSanBayDen()->getMaSB();
                        if (k + 1 < d.size()) f << " (doi " << d[k]->getSanBayDen()->getMaSB() << ")";
                    }
                    f << " [";
                    for (size_t k = 0; k < d.size(); ++k) {
                        if (k) f << "+";
                        f << d[k]->getMaCB();
                    }
                    f << "]\n";
                }
            } else {
                f << "  NOI CHUYEN: khong co\n";
            }
            f << "\n";
        }
    }
    f << "--------------------------------------------------------------------------------\n";
    f << "Tong cap co bay thang: " << coThang << " | Tong cap co noi chuyen: " << coNoi << "\n";
    f.close();
}


void nhapDuLieuMau(QuanLyChuyenBay& ql, QuanLyNguoiDung& nd) {
    // ---- San bay (12) ----
    const char* sb[][3] = {
        {"SGN","Tan Son Nhat","TP HCM"}, {"HAN","Noi Bai","Ha Noi"}, {"DAD","Da Nang","Da Nang"},
        {"PQC","Phu Quoc","Phu Quoc"}, {"CXR","Cam Ranh","Nha Trang"}, {"HPH","Cat Bi","Hai Phong"},
        {"VCA","Can Tho","Can Tho"}, {"VII","Vinh","Vinh"}, {"HUI","Phu Bai","Hue"},
        {"DLI","Lien Khuong","Da Lat"}, {"UIH","Phu Cat","Quy Nhon"}, {"VDO","Van Don","Quang Ninh"},
    };
    for (auto& r : sb) ql.themSanBay(std::make_unique<SanBay>(r[0], r[1], r[2]));

    // ---- May bay ----
    ql.themMayBay(std::make_unique<MayBay>("VN-A001", "A321", 8, 24, 12));
    ql.themMayBay(std::make_unique<MayBay>("VN-A002", "A350", 12, 36, 18));
    ql.themMayBay(std::make_unique<MayBay>("VN-A003", "B787", 12, 40, 20));
    ql.themMayBay(std::make_unique<MayBay>("VJ-B001", "A320", 0, 30, 18));
    ql.themMayBay(std::make_unique<MayBay>("VJ-B002", "A321", 0, 36, 24));
    ql.themMayBay(std::make_unique<MayBay>("QH-C001", "A320", 4, 28, 14));
    ql.themMayBay(std::make_unique<MayBay>("QH-C002", "E190", 4, 20, 10));
    ql.themMayBay(std::make_unique<MayBay>("VU-D001", "A321", 6, 30, 16));

    const auto bay = std::chrono::time_point_cast<std::chrono::minutes>(std::chrono::system_clock::now());
    auto themChuyen = [&](const char* ma, const char* di, const char* den, const char* mb, int diSau, int phut, double gia) {
        const time_point gioDi = bay + std::chrono::minutes(diSau);
        ql.themChuyenBay(std::make_unique<ChuyenBay>(ma, ql.timSanBay(di), ql.timSanBay(den), ql.timMayBay(mb), gioDi,
                                                     gioDi + std::chrono::minutes(phut), gia));
    };
    // VNA
    // Chuyen bay noi dia — gio tinh tu "bay gio" (sau do capNhatGioChuyenTheoThoiGianThuc dong bo lai)
    // diSau = phut ke tu bay gio; phut = thoi gian bay thuc te uoc luong
    themChuyen("VN101", "SGN", "HAN", "VN-A321",  60, 120, 1550000);
    themChuyen("VN102", "SGN", "HAN", "VN-A321N", 150, 120, 1480000);
    themChuyen("VN103", "HAN", "SGN", "VN-B787",  240, 125, 1620000);
    themChuyen("VN104", "HAN", "SGN", "VN-A321",  390, 120, 1500000);
    themChuyen("VN105", "SGN", "HAN", "VN-A330",  540, 125, 1750000);
    themChuyen("VN106", "HAN", "SGN", "VN-A321N", 660, 120, 1520000);
    themChuyen("VN201", "SGN", "DAD", "VN-A321",  90, 75, 920000);
    themChuyen("VN202", "DAD", "HAN", "VN-A321N", 210, 70, 850000);
    themChuyen("VN203", "HAN", "DAD", "VN-A321",  300, 70, 860000);
    themChuyen("VN204", "DAD", "SGN", "VN-A321",  420, 75, 900000);
    themChuyen("VN205", "HAN", "DAD", "VN-A321N",  45, 70, 830000);
    themChuyen("VN301", "SGN", "PQC", "VN-A321", 120, 60, 720000);
    themChuyen("VN302", "PQC", "SGN", "VN-A321", 300, 60, 710000);
    themChuyen("VN311", "SGN", "CXR", "VN-A321N", 180, 55, 680000);
    themChuyen("VN312", "CXR", "SGN", "VN-A321", 360, 55, 690000);
    themChuyen("VN321", "SGN", "VCA", "VN-A321",  90, 45, 520000);
    themChuyen("VN322", "VCA", "SGN", "VN-A321", 240, 45, 510000);
    themChuyen("VN401", "SGN", "HUI", "VN-A321", 225, 75, 800000);
    themChuyen("VN402", "HUI", "SGN", "VN-A321", 450, 75, 810000);
    themChuyen("VN411", "HAN", "VII", "VN-A321N", 60, 45, 550000);
    themChuyen("VN421", "HAN", "VDO", "VN-A321", 360, 45, 580000);
    themChuyen("VJ101", "SGN", "HAN", "VJ-A321",  75, 120, 990000);
    themChuyen("VJ102", "HAN", "SGN", "VJ-A320", 180, 120, 980000);
    themChuyen("VJ201", "SGN", "DAD", "VJ-A320", 120, 75, 650000);
    themChuyen("VJ202", "DAD", "SGN", "VJ-A321", 330, 75, 640000);
    themChuyen("VJ203", "DAD", "HAN", "VJ-A320", 240, 70, 700000);
    themChuyen("VJ204", "HAN", "DAD", "VJ-A321", 420, 70, 710000);
    themChuyen("VJ301", "SGN", "PQC", "VJ-A320", 210, 60, 550000);
    themChuyen("VJ302", "SGN", "DLI", "VJ-A320", 270, 50, 480000);
    themChuyen("VJ303", "DLI", "SGN", "VJ-A320", 480, 50, 490000);
    themChuyen("VJ401", "SGN", "UIH", "VJ-A320", 300, 70, 600000);
    themChuyen("QH101", "HAN", "SGN", "QH-A320",  90, 120, 1200000);
    themChuyen("QH102", "SGN", "HAN", "QH-A320", 300, 120, 1180000);
    themChuyen("QH201", "HAN", "DAD", "QH-E190", 120, 70, 780000);
    themChuyen("QH202", "DAD", "SGN", "QH-A320", 360, 75, 760000);
    themChuyen("QH301", "HAN", "HPH", "QH-E190",  30, 35, 420000);
    themChuyen("VU101", "SGN", "HAN", "VU-A321", 135, 120, 1100000);
    themChuyen("VU102", "HAN", "SGN", "VU-A320", 480, 120, 1090000);
    themChuyen("VU201", "SGN", "CXR", "VU-A320", 240, 55, 620000);

    auto vna = std::make_unique<HangHangKhong>("VNA", "Vietnam Airlines");
    for (const char* m : {"VN-A321","VN-A321N","VN-B787","VN-A330"}) vna->themMayBay(ql.timMayBay(m));
    for (const char* m : {"VN101","VN102","VN103","VN104","VN105","VN106","VN201","VN202","VN203","VN204","VN205","VN301","VN302","VN311","VN312","VN321","VN322","VN401","VN402","VN411","VN421"})
        vna->themChuyenBay(ql.timChuyenBay(m));
    ql.themHang(std::move(vna));

    auto vj = std::make_unique<HangHangKhong>("VJ", "VietJet Air");
    for (const char* m : {"VJ-A320","VJ-A321","VJ-A330"}) vj->themMayBay(ql.timMayBay(m));
    for (const char* m : {"VJ101","VJ102","VJ201","VJ202","VJ203","VJ204","VJ301","VJ302","VJ303","VJ401"})
        vj->themChuyenBay(ql.timChuyenBay(m));
    ql.themHang(std::move(vj));

    auto qh = std::make_unique<HangHangKhong>("QH", "Bamboo Airways");
    for (const char* m : {"QH-A320","QH-E190"}) qh->themMayBay(ql.timMayBay(m));
    for (const char* m : {"QH101","QH102","QH201","QH202","QH301"})
        qh->themChuyenBay(ql.timChuyenBay(m));
    ql.themHang(std::move(qh));

    auto vu = std::make_unique<HangHangKhong>("VU", "Vietravel Airlines");
    for (const char* m : {"VU-A320","VU-A321"}) vu->themMayBay(ql.timMayBay(m));
    for (const char* m : {"VU101","VU102","VU201"})
        vu->themChuyenBay(ql.timChuyenBay(m));
    ql.themHang(std::move(vu));

    nd.themNguoiDung(std::make_unique<Admin>("AD01", "Quản trị viên", "admin123"));
    nd.themNguoiDung(std::make_unique<DaiLy>("DL01", "Đại lý Sao Việt", "daily123", 0.05));
    nd.themNguoiDung(std::make_unique<DaiLy>("DL02", "Vietnam Booking", "daily123", 0.04));
    nd.themNguoiDung(std::make_unique<DaiLy>("DL03", "BestPrice Travel", "daily123", 0.035));
    nd.themNguoiDung(std::make_unique<DaiLy>("DL04", "Sao Mai Travel", "daily123", 0.045));
    nd.themNguoiDung(std::make_unique<DaiLy>("DL05", "Hong Ngoc Ha Travel", "daily123", 0.06));
    // Hanh khach: tu dang ky (khong tao san HK01/HK02)
}

}


// ======================================================================
// PHAN 5: WEB DEMO (khong thuoc UML) - may chu HTTP nho chi goi ham PUBLIC cua 34 lop o tren
// ======================================================================
#if BAT_WEB_DEMO
#ifdef _WIN32
  typedef SOCKET sock_t;
  #define DONG_SOCK closesocket
#else
  typedef int sock_t;
  #define DONG_SOCK close
  #define INVALID_SOCKET (-1)
#endif

namespace web {
const std::string& trangChu() {
    static const std::string s = std::string(R"HTML(<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Airline System Management  Demo BTL OOP T04</title>
<style>
:root{--bg:#f4f7fb;--card:#fff;--tx:#14213d;--mut:#64748b;--line:#e2e8f0;--pri:#1d4ed8;--pri2:#dbeafe;--ok:#15803d;--okbg:#dcfce7;--warn:#b45309;--warnbg:#fef3c7;--bad:#b91c1c;--badbg:#fee2e2;--tg:#7c3aed;--r:12px}
*{box-sizing:border-box}
body{margin:0;font:15px/1.5 system-ui,-apple-system,"Segoe UI",Roboto,sans-serif;background:var(--bg);color:var(--tx)}
header{background:linear-gradient(135deg,#1e3a8a,#1d4ed8 60%,#0ea5e9);color:#fff;padding:14px 24px;display:flex;align-items:center;gap:16px;flex-wrap:wrap}
header h1{margin:0;font-size:20px;letter-spacing:.3px}header small{opacity:.85;display:block;font-weight:400;font-size:12px}
.sp{flex:1}.chip{background:rgba(255,255,255,.18);padding:6px 12px;border-radius:99px;font-size:13px}
main{max-width:1120px;margin:20px auto;padding:0 16px}
.card{background:var(--card);border:1px solid var(--line);border-radius:var(--r);padding:18px;margin-bottom:16px;box-shadow:0 1px 2px rgba(0,0,0,.04)}
h2{margin:0 0 12px;font-size:17px}h3{margin:14px 0 8px;font-size:15px}
nav{display:flex;gap:6px;flex-wrap:wrap;margin-bottom:16px}
nav button{border:1px solid var(--line);background:#fff;padding:9px 14px;border-radius:99px;cursor:pointer;font-size:14px;color:var(--tx)}
nav button.on{background:var(--pri);border-color:var(--pri);color:#fff}
button.b{background:var(--pri);color:#fff;border:0;padding:9px 16px;border-radius:8px;cursor:pointer;font-size:14px;font-weight:600}
button.b:hover{filter:brightness(1.08)}button.b:disabled{opacity:.5;cursor:not-allowed}
button.b.g{background:#fff;color:var(--pri);border:1px solid var(--pri)}button.b.r{background:var(--bad)}button.b.s{padding:5px 10px;font-size:13px}
label{display:block;font-size:12px;color:var(--mut);margin-bottom:3px}
input,select{width:100%;padding:8px 10px;border:1px solid #cbd5e1;border-radius:8px;font-size:14px;background:#fff}
.grid{display:grid;gap:12px;grid-template-columns:repeat(auto-fit,minmax(160px,1fr));align-items:end}
table{width:100%;border-collapse:collapse;font-size:14px}th{text-align:left;color:var(--mut);font-weight:600;font-size:12px;text-transform:uppercase;padding:8px;border-bottom:2px solid var(--line)}
td{padding:10px 8px;border-bottom:1px solid var(--line);vertical-align:middle}tr:hover td{background:#f8fafc}
.tag{display:inline-block;padding:2px 9px;border-radius:99px;font-size:12px;font-weight:600}
.t-tg{background:#ede9fe;color:var(--tg)}.t-pt{background:var(--pri2);color:var(--pri)}.t-tk{background:var(--okbg);color:var(--ok)}
.t-ok{background:var(--okbg);color:var(--ok)}.t-wait{background:var(--warnbg);color:var(--warn)}
.acc{display:grid;grid-template-columns:repeat(auto-fit,minmax(210px,1fr));gap:12px}
.acc button{text-align:left;background:#fff;border:1px solid var(--line);border-radius:var(--r);padding:16px;cursor:pointer;font-size:14px}
.acc button:hover{border-color:var(--pri);box-shadow:0 4px 14px rgba(29,78,216,.15)}
.acc b{display:block;font-size:16px}.acc span{color:var(--mut);font-size:13px}
.two{display:grid;grid-template-columns:1.5fr 1fr;gap:16px}@media(max-width:820px){.two{grid-template-columns:1fr}}
.seatmap{overflow-x:auto}.cabin{margin-bottom:14px}.cabin-title{font-weight:700;margin-bottom:6px}.cabin-title small{color:var(--mut);font-weight:400}
.row{display:flex;align-items:center;gap:5px;margin-bottom:5px}.rn{width:22px;text-align:right;color:var(--mut);font-size:12px;margin-right:4px}
.aisle{width:22px}
.seat{width:34px;height:34px;border-radius:7px 7px 4px 4px;border:1.5px solid #86efac;background:var(--okbg);color:var(--ok);font-weight:700;font-size:13px;cursor:pointer;padding:0}
.seat.held{background:var(--warnbg);border-color:#fcd34d;color:var(--warn);cursor:not-allowed}
.seat.taken{background:#e2e8f0;border-color:#cbd5e1;color:#94a3b8;cursor:not-allowed}
.seat.sel{background:var(--pri);border-color:var(--pri);color:#fff}.seat.cur{outline:3px solid #f59e0b}
.legend{display:flex;gap:14px;font-size:12px;color:var(--mut);margin:8px 0;flex-wrap:wrap}.legend i{display:inline-block;width:12px;height:12px;border-radius:3px;margin-right:4px;vertical-align:-1px}
.kv{display:grid;grid-template-columns:auto 1fr;gap:4px 12px;font-size:14px;margin:10px 0}.kv span{color:var(--mut)}
.big{font-size:26px;font-weight:800}.cd{font-size:30px;font-weight:800;color:var(--warn);font-variant-numeric:tabular-nums}
.hood{display:none!important;background:#0f172a;color:#cbd5e1;border-radius:10px;padding:10px 14px;margin-top:12px;font:12.5px/1.7 ui-monospace,Consolas,monospace}
.hood b{color:#7dd3fc}.hood em{color:#86efac;font-style:normal}
.ticket{border:1px solid var(--line);border-radius:var(--r);padding:14px;margin-bottom:12px;background:#fff;border-left:5px solid var(--pri)}
.ticket.tg{border-left-color:var(--tg)}.ticket.tk{border-left-color:var(--ok)}
.ticket .h{display:flex;gap:10px;align-items:center;flex-wrap:wrap;margin-bottom:6px}.ticket .h b{font-size:16px}
.acts{display:flex;gap:8px;margin-top:10px;flex-wrap:wrap}
.leg{display:flex;align-items:center;gap:10px;padding:8px 0;border-bottom:1px dashed var(--line);flex-wrap:wrap}.leg:last-child{border:0}
.path{border:1px solid var(--line);border-radius:var(--r);padding:12px 14px;margin-bottom:10px}
.chips{display:flex;gap:6px;flex-wrap:wrap}.chips .tag{background:var(--pri2);color:var(--pri);cursor:default}
.empty{color:var(--mut);padding:18px;text-align:center}
.bar{height:22px;border-radius:6px;background:var(--pri);color:#fff;font-size:12px;padding:0 8px;line-height:22px;min-width:26px}
.stat{display:grid;grid-template-columns:repeat(auto-fit,minmax(190px,1fr));gap:12px}.stat .card{margin:0;text-align:center}.stat small{color:var(--mut)}
#toast{position:fixed;right:18px;bottom:18px;display:flex;flex-direction:column;gap:8px;z-index:99}
#toast div{padding:11px 16px;border-radius:10px;color:#fff;box-shadow:0 6px 20px rgba(0,0,0,.2);max-width:380px;font-size:14px}
.m-ok{background:var(--ok)}.m-ok,.m-err{white-space:pre-line;max-width:min(420px,92vw);line-height:1.45}.m-err{background:var(--bad)}
.modal{position:fixed;inset:0;background:rgba(15,23,42,.55);display:flex;align-items:center;justify-content:center;z-index:50;padding:16px}
.modal>div{background:#fff;border-radius:var(--r);padding:20px;max-width:560px;width:100%;max-height:90vh;overflow:auto}
pre.bp{background:#0f172a;color:#e2e8f0;padding:14px;border-radius:10px;font:13px/1.5 ui-monospace,Consolas,monospace;overflow:auto}
footer{text-align:center;color:var(--mut);font-size:12px;padding:20px}
.right{text-align:right}.mut{color:var(--mut)}.mb0{margin-bottom:0}
</style>
</head>
<body>
<header>
  <div><h1>Airline System Management <small>Demo BTL OOP DT04 – Quản lý đặt vé &amp; điều hành chuyến bay</small></h1></div>
  <div class="sp"></div>
  <div id="who"></div>
</header>
<main id="app"></main>
<footer>Airline System Management — Demo BTL OOP</footer>
<div id="toast"></div>
<div id="modal"></div>

<script>
const $ = (s, r = document) => r.querySelector(s);
const $$ = (s, r = document) => [...r.querySelectorAll(s)];
const esc = s => String(s ?? '').replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const vnd = n => Number(n).toLocaleString('vi-VN') + ' ';
const hm = s => { const [d, t] = s.split(' '); const [y, m, dd] = d.split('-'); return `${t}  ${dd}/${m}`; };
const dur = p => `${Math.floor(p / 60)}g${String(p % 60).padStart(2, '0')}`;
const HANG = { TG: 'Thương gia', PT: 'Phổ thông', TK: 'Tiết kiệm' };
const HESO = { TG: 2.5, PT: 1, TK: 0.8 };
const tagH = h => `<span class="tag t-${h.toLowerCase()}">${HANG[h]}</span>`;
let token = sessionStorage.getItem('token'), user = null, tab = null, meta = { sanBay: [], mayBay: [] };
const S = { flights: [], sel: null, ma: null, ghe: [], hold: null, timer: null, legs: [], hanh: 'PT' };

async function api(path, params = {}, method = 'GET') {
  const p = new URLSearchParams();
  for (const [k, v] of Object.entries(params)) if (v !== undefined && v !== null && v !== '') p.append(k, v);
  if (token) p.set('token', token);
  let url = '/api/' + path; const opt = { method };
  if (method === 'GET') { const q = p.toString(); if (q) url += '?' + q; }
  else { opt.headers = { 'Content-Type': 'application/x-www-form-urlencoded' }; opt.body = p.toString(); }
  const r = await fetch(url, opt); const d = await r.json();
  if (!d.ok) throw new Error(d.loi || 'Lỗi không xác định');
  return d;
}
const post = (p, a) => api(p, a, 'POST');
function toast(msg, ok = true) {
  const d = document.createElement('div'); d.className = ok ? 'm-ok' : 'm-err'; d.textContent = msg;
  $('#toast').appendChild(d); setTimeout(() => d.remove(), ok ? 3500 : 10000);
}
const guard = fn => async (...a) => { try { await fn(...a); } catch (e) { toast(e.message, false); } };
function modal(html) { $('#modal').innerHTML = `<div class="modal" onclick="if(event.target===this)closeModal()"><div>${html}</div></div>`; }
function closeModal() { $('#modal').innerHTML = ''; }
const hood = lines => '';

/* ---------------- ĐĂNG NHẬP ---------------- */
function loginView() {
  $('#who').innerHTML = '';
  $('#app').innerHTML = `
  <div class="card"><h2>Chọn tài khoản có sẵn</h2>
    <div class="acc">
      <button data-u="AD01|admin123"><b>Quản trị viên</b><span>Tài khoản: AD01 — Mật khẩu: admin123</span></button>
      <button data-u="DL01|daily123"><b>Đại lý Sao Việt</b><span>Tài khoản: DL01 — Mật khẩu: daily123</span></button>
      <button data-u="DL02|daily123"><b>Vietnam Booking</b><span>Tài khoản: DL02 — Mật khẩu: daily123</span></button>
      <button data-u="DL03|daily123"><b>BestPrice Travel</b><span>Tài khoản: DL03 — Mật khẩu: daily123</span></button>
      <button data-u="DL04|daily123"><b>Sao Mai Travel</b><span>Tài khoản: DL04 — Mật khẩu: daily123</span></button>
      <button data-u="DL05|daily123"><b>Hong Ngoc Ha Travel</b><span>Tài khoản: DL05 — Mật khẩu: daily123</span></button>
    </div>
    <p class="mut">Hành khách: đăng ký tài khoản mới ở form bên dưới.</p>
  </div>
  <div class="card"><h2>Hoặc đăng nhập thủ công</h2>
    <form id="lf" class="grid">
      <div><label>Mã người dùng</label><input name="ma" autocomplete="off"></div>
      <div><label>Mật khẩu</label><input name="mk" type="password"></div>
      <div><button class="b" type="submit">Đăng nhập</button></div>
    </form></div>
  <div class="card"><h2>Đăng ký Hành khách (tự tạo tài khoản)</h2>
    <p class="mut" style="margin-top:0">Chỉ Hành khách được tự đăng ký. Admin và Đại lý chỉ dùng tài khoản có sẵn.</p>
    <form id="rf" class="grid">
      <div><label>Mã (vd HK01)</label><input name="ma" required></div>
      <div><label>Họ tên</label><input name="ten" required></div>
      <div><label>Mật khẩu</label><input name="mk" type="password" required></div>
      <div><label>CCCD (9 hoặc 12 số)</label><input name="cccd" required placeholder="001234567890"></div>
      <div><label>Số điện thoại</label><input name="sdt" required placeholder="0901234567"></div>
      <div><label>Email</label><input name="email" required placeholder="ten@gmail.com"></div>
      <div><button class="b" type="submit">Đăng ký</button></div>
    </form></div>`;
  $$('.acc button').forEach(b => b.onclick = guard(() => { const [ma, mk] = b.dataset.u.split('|'); return login(ma, mk); }));
  $('#lf').onsubmit = guard(e => { e.preventDefault(); const f = new FormData(e.target); return login(f.get('ma'), f.get('mk')); });
  $('#rf').onsubmit = guard(async e => {
    e.preventDefault();
    const f = Object.fromEntries(new FormData(e.target));
    await post('dangky', f);
    toast('Đăng ký thành công! Hãy đăng nhập bằng mã ' + f.ma);
    e.target.reset();
  });
}
async function login(ma, mk) {
  const d = await post('login', { ma, mk }); token = d.token; user = d.user; sessionStorage.setItem('token', token);
  toast('Xin chào ' + user.ten); await boot();
}
async function logout() { try { await post('logout'); } catch (e) {} token = null; user = null; sessionStorage.removeItem('token'); clearInterval(S.timer); S.hold = null; loginView(); }

/* ---------------- KHUNG CHNH ---------------- */
const VAITRO = {HK:"Hành khách",DL:"Đại lý",AD:"Admin"};
const TABS = {
  HK: [['tim', 'Tìm chuyến'], ['noi', 'Nối chuyến'], ['dat', 'Đặt vé'], ['ht', 'Hành trình (Builder)'], ['ss', 'So sánh đại lý'], ['ve', 'Vé của tôi']],
  DL: [['tim', 'Tìm chuyến'], ['noi', 'Nối chuyến'], ['dat', 'Đặt vé hộ'], ['ht', 'Hành trình (Builder)'], ['ve', 'Vé đã bán'], ['tk', 'Hoa hồng']],
  AD: [['qly', 'Chuyến bay'], ['ve', 'Tất cả vé'], ['daily', 'Quản lý đại lý'], ['tk', 'Thống kê']]
};
async function boot() {
  if (!user) { try { user = (await api('me')).user; } catch (e) { token = null; sessionStorage.removeItem('token'); return loginView(); } }
  meta = await api('meta');
  $('#who').innerHTML = `<span class="chip">${VAITRO[user.vaiTro]} — ${esc(user.ten)} (${esc(user.ma)})</span> <button class="b g s" id="lo">Đăng xuất</button>`;
  $('#lo').onclick = logout;
  $('#app').innerHTML = `<nav id="nav"></nav><div id="panel"></div>`;
)HTML") +
        R"HTML(  $('#nav').innerHTML = TABS[user.vaiTro].map(([k, t]) => `<button data-t="${k}">${t}</button>`).join('');
  $$('#nav button').forEach(b => b.onclick = () => go(b.dataset.t));
  await go(TABS[user.vaiTro][0][0]);
}
async function go(t) {
  tab = t; $$('#nav button').forEach(b => b.classList.toggle('on', b.dataset.t === t));
  clearInterval(S.timer);
  try { await ({ tim: pTim, noi: pNoi, dat: pDat, ht: pHT, ve: pVe, tk: pTK, qly: pQly, ss: pSS, daily: pDaily })[t](); } catch (e) { toast(e.message, false); }
}
const apOpts = (all = true) => (all ? '<option value="">Tất cả</option>' : '') + meta.sanBay.map(s => `<option value="${s.ma}">${s.ma}  ${esc(s.tp)}</option>`).join('');

/* ---------------- TÌM CHUYẾN ---------------- */
function flightRows(list, action) {
  if (!list.length) return '<div class="empty">Không có chuyến bay phù hợp</div>';
  return `<table><tr><th>Mã</th><th>Tuyến</th><th>Khởi hành</th><th>Đến</th><th>Bay</th><th class="right">Giá cơ bản</th><th>Ghế trống</th><th></th></tr>` + list.map(c => `
    <tr><td><b>${c.ma}</b><br><small class="mut">${c.mayBay}</small></td><td>${c.di} → ${c.den}<br><small class="mut">${esc(c.diTen)} → ${esc(c.denTen)}</small></td>
    <td>${hm(c.gioDi)}</td><td>${hm(c.gioDen)}</td><td>${dur(c.phut)}</td><td class="right"><b>${vnd(c.gia)}</b></td>
    <td><span class="tag t-tg">TG ${c.TG.trong}/${c.TG.tong}</span> <span class="tag t-pt">PT ${c.PT.trong}/${c.PT.tong}</span> <span class="tag t-tk">TK ${c.TK.trong}/${c.TK.tong}</span></td>
    <td class="right">${action(c)}</td></tr>`).join('') + '</table>';
}
async function pTim() {
  const today = new Date(); const ymd = today.toISOString().slice(0,10);
  $('#panel').innerHTML = `<div class="card"><h2>Tìm chuyến bay</h2>
    <p class="mut">Danh sách chuyến theo ngày thực tế (giờ hệ thống đã căn theo "bây giờ"). Chọn điểm đi/đến rồi bấm Tìm.</p>
    <form id="f" class="grid">
    <div><label>Điểm đi</label><select name="di">${apOpts()}</select></div>
    <div><label>Điểm đến</label><select name="den">${apOpts()}</select></div>
    <div><label>Ngày bay</label><input type="date" name="ngay" value="${ymd}"></div>
    <div><label>Sắp xếp</label><select name="sapxep"><option value="gio">Theo giờ đi</option><option value="gia">Theo giá</option></select></div>
    <div><button class="b" type="submit">Tìm chuyến</button></div></form></div>
    <div class="card" id="res"><div class="empty">Đang tải danh sách…</div></div>`;
  const run = guard(async () => {
    const f = Object.fromEntries(new FormData($('#f')));
    const d = await api('chuyenbay', f);
    if (!d.chuyenBay.length) {
      $('#res').innerHTML = '<div class="empty">Không có chuyến phù hợp. Thử bỏ lọc ngày hoặc đổi điểm đi/đến.</div>';
      return;
    }
    $('#res').innerHTML = `<div class="mut" style="margin-bottom:8px">Có <b>${d.chuyenBay.length}</b> chuyến — chọn "Chọn ghế / Đặt vé" để đặt.</div>` +
      flightRows(d.chuyenBay, c => `<button class="b s" data-m="${c.ma}">Chọn ghế / Đặt vé</button>`);
    $$('#res [data-m]').forEach(b => b.onclick = () => { S.ma = b.dataset.m; go('dat'); });
  });
  $('#f').onsubmit = e => { e.preventDefault(); run(); }; run();
}

/* ---------------- NỐI CHUYẾN ---------------- */
async function pNoi() {
  const today = new Date(); const ymd = today.toISOString().slice(0,10);
  $('#panel').innerHTML = `<div class="card"><h2>Tìm hành trình nối chuyến</h2>
    <p class="mut">Hiện <b>tất cả</b> phương án nối chuyến (tối đa 3 chặng). Bấm <b>Đặt cả hành trình</b> để đặt full các chặng và tính <b>tổng tiền</b>.</p>
    <form id="f" class="grid">
    <div><label>Điểm đi</label><select name="di">${apOpts(false)}</select></div>
    <div><label>Điểm đến</label><select name="den">${apOpts(false)}</select></div>
    <div><label>Ngày bay</label><input type="date" name="ngay" value="${ymd}"></div>
    <div><label>Chờ tối thiểu (phút)</label><input type="number" name="cho" value="45" min="0"></div>
    <div><button class="b" type="submit">Tìm nối chuyến</button></div></form></div>
    <div id="res"><div class="card empty">Chọn điểm đi/đến rồi bấm tìm.</div></div>`;
  const sel = $('#f [name=den]'); if (sel.options.length > 1) sel.selectedIndex = 1;
  const run = guard(async () => {
    const f = Object.fromEntries(new FormData($('#f')));
    const d = await api('noichuyen', f);
    if (!d.hanhTrinh.length) {
      $('#res').innerHTML = '<div class="card empty">Không có hành trình nối chuyến phù hợp. Thử giảm thời gian chờ hoặc đổi ngày.</div>';
      return;
    }
    $('#res').innerHTML = `<div class="mut" style="margin:8px 0">Có <b>${d.hanhTrinh.length}</b> phương án nối chuyến:</div>` +
      d.hanhTrinh.map((h, i) => {
        const rows = (h.chang || []).map((c, j) => {
          const tg = (c.TG && c.TG.trong != null) ? c.TG.trong : '?';
          const pt = (c.PT && c.PT.trong != null) ? c.PT.trong : '?';
          const tk = (c.TK && c.TK.trong != null) ? c.TK.trong : '?';
          return `<tr>
            <td>Chặng ${j+1}</td><td><b>${c.ma}</b></td>
            <td>${c.di} → ${c.den}<br><small class="mut">${esc(c.diTen||'')} → ${esc(c.denTen||'')}</small></td>
            <td>${hm(c.gioDi)}</td><td>${hm(c.gioDen)}</td>
            <td class="right">${vnd(c.gia)}</td>
            <td>${tg}/${pt}/${tk}</td></tr>`;
        }).join('');
        const mas = (h.chang||[]).map(c => c.ma).join(',');
        const tong = h.tongGia || 0;
        return `<div class="card path"><div style="display:flex;flex-wrap:wrap;align-items:center;gap:10px;justify-content:space-between">
          <div><b>Phương án ${i+1}</b> — ${(h.chang||[]).length} chặng — Tổng giá CB: <b class="big">${vnd(tong)}</b></div>
          <button class="b" type="button" data-legs="${mas}" data-tong="${tong}">Đặt cả hành trình</button>
          </div>
          <table style="margin-top:8px"><tr><th></th><th>Mã</th><th>Tuyến</th><th>Đi</th><th>Đến</th><th class="right">Giá</th><th>Ghế TG/PT/TK</th></tr>${rows}</table></div>`;
      }).join('');
    $$('#res [data-legs]').forEach(b => b.onclick = async () => {
      const mas = (b.dataset.legs || '').split(',').filter(Boolean);
      if (!mas.length) return toast('Không có chặng', false);
      if (!S.flights || !S.flights.length) {
        try { const all = await api('chuyenbay', { sapxep: 'gio' }); S.flights = all.chuyenBay || []; } catch (e) {}
      }
      S.legs = mas;
      S.hanh = S.hanh || 'PT';
      toast('Đã chọn ' + mas.length + ' chặng — tổng CB ' + vnd(+b.dataset.tong || 0));
      go('ht');
    });
  });
  $('#f').onsubmit = e => { e.preventDefault(); run(); }; run();
}


function seatMap(ghe, opt) {
  opt = opt || {};
  const sel = opt.sel || null, off = !!opt.off;
  const byHang = { TG: [], PT: [], TK: [] };
  (ghe || []).forEach(g => { if (byHang[g.hang]) byHang[g.hang].push(g); });
  const labels = { TG: 'Thương gia', PT: 'Phổ thông', TK: 'Tiết kiệm' };
  let h = '<div class="seatmap">';
  ['TG','PT','TK'].forEach(hg => {
    const list = byHang[hg];
    if (!list || !list.length) return;
    h += `<div class="cabin"><div class="cabin-title">${labels[hg]} <small>(${list.filter(x=>x.tt==='T').length} trống)</small></div>`;
    const rows = {};
    list.forEach(g => {
      const m = String(g.so).match(/^(\d+)([A-Z])$/);
      if (!m) return;
      const rn = m[1], col = m[2];
      if (!rows[rn]) rows[rn] = {};
      rows[rn][col] = g;
    });
    Object.keys(rows).sort((a,b)=>+a-+b).forEach(rn => {
      h += `<div class="row"><span class="rn">${rn}</span>`;
      const cols = Object.keys(rows[rn]).sort();
      const mid = Math.ceil(cols.length / 2);
      cols.forEach((col, i) => {
        const g = rows[rn][col];
        const c = ['seat'];
        if (g.tt === 'T') c.push('free');
        else if (g.tt === 'H') c.push('hold');
        else c.push('taken');
        if (sel === g.so) c.push('sel');
        const ok = g.tt === 'T' && !off;
        h += `<button type="button" class="${c.join(' ')}" data-so="${g.so}" ${ok?'':'disabled'}>${col}</button>`;
        if (i === mid - 1) h += '<span class="aisle"></span>';
      });
      h += '</div>';
    });
    h += '</div>';
  });
  h += '</div><div class="legend"><span><i style="background:#dcfce7;border:1px solid #86efac"></i>Trống</span>'
    + '<span><i style="background:#fef3c7;border:1px solid #fcd34d"></i>Giữ chỗ</span>'
    + '<span><i style="background:#e2e8f0"></i>Đã đặt</span>'
    + '<span><i style="background:#1d4ed8"></i>Đang chọn</span></div>';
  return h;
}

/* ---------------- ĐẶT VÉ ---------------- */
async function pDat() {
  const all = await api('chuyenbay', { sapxep: 'gio' }); S.flights = all.chuyenBay;
  S.sel = null; S.hold = null;
  const dl = user.vaiTro === 'DL';
  const daChon = S.ma && S.flights.find(c => c.ma === S.ma);
  const tenDl = S.tenDaiLy || S.maDaiLy || '';
  const banner = S.maDaiLy
    ? `<div class="card" style="border-color:#93c5fd;background:#eff6ff"><h2 style="margin:0 0 6px">Đặt vé hộ qua đại lý: ${esc(tenDl)} <small class="mut">(${esc(S.maDaiLy)})</small></h2>
         <p class="mut" style="margin:0">Bạn đang đặt vé thông qua đại lý này (hoa hồng ghi nhận cho đại lý). <a href="#" id="boDl">Bỏ đại lý — đặt tại quầy</a></p></div>`
    : (dl ? '' : `<p class="mut">Đặt trực tiếp tại quầy (không qua đại lý).</p>`);

  $('#panel').innerHTML = `
    ${banner}
    <div class="card"><h2>Đặt vé — tìm chuyến</h2>
      <p class="mut">Chọn điểm đi và điểm đến, bấm Tìm chuyến, rồi chọn 1 chuyến trong list để đặt ghế.</p>
      <form id="fTimDat" class="grid">
        <div><label>Điểm đi</label><select name="di" id="diDat">${apOpts(true)}</select></div>
        <div><label>Điểm đến</label><select name="den" id="denDat">${apOpts(true)}</select></div>
        <div><label>&nbsp;</label><button class="b" type="submit">Tìm chuyến</button></div>
      </form>
      <div id="listDat" style="margin-top:14px"></div>
    </div>
    <div class="two" id="datArea" style="display:none;margin-top:14px">
      <div class="card"><h2>${S.maDaiLy ? 'Chọn ghế — đặt hộ «' + esc(tenDl) + '»' : 'Chọn ghế'} — <span id="maDatLabel"></span></h2>
        <button type="button" class="b ghost" id="btnDoiChuyen" style="margin-bottom:10px">← Đổi chuyến</button>
        <div id="map"></div>
      </div>
      <div class="card"><h2>${dl ? 'Đặt vé hộ khách' : (S.maDaiLy ? 'Thanh toán qua đại lý' : 'Đặt vé')}</h2><div id="box"></div></div>
    </div>`;

  const bo = $('#boDl'); if (bo) bo.onclick = e => { e.preventDefault(); S.maDaiLy = ''; S.tenDaiLy = ''; S.fromSS = false; pDat(); };

  const hienList = (ds) => {
    const box = $('#listDat');
    if (!ds.length) {
      box.innerHTML = '<div class="empty">Không có chuyến bay phù hợp. Thử đổi điểm đi / điểm đến.</div>';
      return;
    }
    box.innerHTML = `<table><tr><th>STT</th><th>Mã</th><th>Tuyến</th><th>Giờ đi</th><th>Giờ đến</th><th class="right">Giá CB</th><th>Ghế TG/PT/TK</th><th></th></tr>
      ${ds.map((c,i) => `<tr>
        <td>${i+1}</td><td><b>${c.ma}</b></td>
        <td>${c.di}→${c.den}</td>
        <td>${hm(c.gioDi)}</td><td>${hm(c.gioDen)}</td>
        <td class="right">${vnd(c.gia)}</td>
        <td>${(c.TG&&c.TG.trong)||0}/${(c.PT&&c.PT.trong)||0}/${(c.TK&&c.TK.trong)||0}</td>
        <td><button type="button" class="b" data-m="${c.ma}">Chọn đặt</button></td>
      </tr>`).join('')}</table>`;
    $$('#listDat [data-m]').forEach(b => b.onclick = guard(async () => {
      S.ma = b.dataset.m; S.sel = null; S.hold = null;
      $('#datArea').style.display = '';
      $('#maDatLabel').textContent = S.ma;
      await loadMap();
    }));
  };

  $('#fTimDat').onsubmit = e => {
    e.preventDefault();
    const di = $('#diDat').value, den = $('#denDat').value;
    let ds = S.flights.slice();
    if (di) ds = ds.filter(c => c.di === di);
    if (den) ds = ds.filter(c => c.den === den);
    $('#datArea').style.display = 'none';
    S.ma = null; S.sel = null; S.hold = null;
    hienList(ds);
  };
  $('#btnDoiChuyen').onclick = () => {
    $('#datArea').style.display = 'none';
    S.ma = null; S.sel = null; S.hold = null;
  };

  // Tu So sanh dai ly: mo thang chon ghe cho chuyen da chon
  if (daChon) {
    const c = S.flights.find(x => x.ma === S.ma);
    if (c) {
      $('#diDat').value = c.di;
      $('#denDat').value = c.den;
      hienList(S.flights.filter(x => x.di === c.di && x.den === c.den));
      $('#datArea').style.display = '';
      $('#maDatLabel').textContent = S.ma;
      await loadMap();
    }
  }
}


async function loadMap() {
  if (!S.ma) { $('#map').innerHTML = '<div class="empty">Chưa có chuyến bay</div>'; return; }
  const d = await api('sodoghe', { ma: S.ma }); S.ghe = d.ghe; S.cb = d.chuyenBay; drawMap(); drawBox();
}
function drawMap() {
  $('#map').innerHTML = seatMap(S.ghe, { sel: S.sel, off: !!S.hold });
  $$('#map .seat').forEach(b => b.onclick = () => { S.sel = b.dataset.so; drawMap(); drawBox(); });
}
function drawBox() {
  const dl = user.vaiTro === 'DL', box = $('#box'); if (!box) return;
  if (S.hold) return holdBox();
  const g = S.ghe.find(x => x.so === S.sel);
  if (!g) { box.innerHTML = '<div class="empty">Bấm vào một ghế trống trên sơ đồ để đặt</div>'; return; }
  box.innerHTML = `<div class="kv">
      <span>Chuyến</span><b>${S.cb.ma}  ${S.cb.di}→${S.cb.den}</b>
      <span>Khởi hành</span><b>${hm(S.cb.gioDi)}</b>
      <span>Ghế</span><b>${g.so}</b>
      <span>Hạng vé</span><b>${tagH(g.hang)}</b></div>
    <div class="mut">Giá dự kiến (giá cơ bản × ${HESO[g.hang]})</div>
    <div class="big">${vnd(S.cb.gia * HESO[g.hang])}</div>
    ${dl ? '<div style="margin-top:10px"><label>Mã hành khách</label><input id="mk" placeholder="VD: HK01"></div>' : ''}
    <label style="margin-top:10px"><input type="checkbox" id="fast" style="width:auto"> Demo nhanh: giữ chỗ chỉ <b>15 giây</b> (xem hết hạn)</label>
    <div class="acts"><button class="b" id="gc">Giữ chỗ</button></div>`;
  $('#gc').onclick = guard(async () => {
    const d = await post('giucho', { maCB: S.ma, soGhe: S.sel, maKhach: dl ? $('#mk').value : '', maDaiLy: (!dl && S.maDaiLy) ? S.maDaiLy : '', giay: $('#fast').checked ? 15 : '' });
    S.hold = { ...d, until: Date.now() + d.conLai * 1000, maKhach: dl ? $('#mk').value : '' };
    toast('Đã giữ ghế ' + d.soGhe); await loadMap(); startTimer();
  });
}
function startTimer() { clearInterval(S.timer); S.timer = setInterval(tick, 250); tick(); }
function tick() {
  const e = $('#cd'); if (!e || !S.hold) return clearInterval(S.timer);
  const left = Math.max(0, Math.ceil((S.hold.until - Date.now()) / 1000));
  e.textContent = `${Math.floor(left / 60)}:${String(left % 60).padStart(2, '0')}`;
  if (left === 0) { $('#st').innerHTML = '<span class="tag t-wait">Hết thời gian giữ chỗ</span>'; }
}
function holdBox() {
  const h = S.hold;
  $('#box').innerHTML = `<div class="kv"><span>Chuyến</span><b>${h.maCB}</b><span>Ghế</span><b>${h.soGhe}</b><span>Hạng vé</span><b>${tagH(h.hang)}</b></div>
    <div class="mut">Thời gian giữ chỗ còn lại</div><div class="cd" id="cd">--:--</div><div id="st"><span class="tag t-ok">Đang giữ chỗ</span></div>
    <div class="acts"><button class="b" id="pay">Thanh toán</button><button class="b g" id="cancel">Hủy giữ chỗ</button></div>`;
  $('#pay').onclick = guard(async () => {
    const id = S.hold.holdId; clearInterval(S.timer);
    try { const d = await post('thanhtoan', { holdId: id }); S.hold = null; S.sel = null; toast('Đặt vé thành công ' + d.ve.ma); await loadMap(); $('#box').innerHTML = ticketHtml(d.ve, true) + '<div class="acts"><button class="b g" onclick="go(\'ve\')">Xem vé</button></div>'; }
    catch (e) { S.hold = null; S.sel = null; await loadMap(); throw e; }
  });
  $('#cancel').onclick = guard(async () => { clearInterval(S.timer); await post('huygiucho', { holdId: S.hold.holdId }); S.hold = null; S.sel = null; toast('Đã hủy giữ chỗ'); await loadMap(); });
  startTimer();
}

/* ---------------- V ---------------- */
function chain(){ return ""; }

function ticketHtml(v, withHood, acts) {
  const dv = v.dichVu.map(d => `<div>+ ${esc(d)}</div>`).join('');
  return `<div class="ticket ${v.hang === 'TG' ? 'tg' : v.hang === 'TK' ? 'tk' : ''}"><div class="h"><b>${v.ma}</b>${tagH(v.hang)}
      ${v.checkIn ? '<span class="tag t-ok">✓ check-in</span>' : '<span class="tag t-wait">Chưa check-in</span>'}<span class="sp"></span><b>${vnd(v.gia)}</b></div>
    <div><b>${v.maCB}</b>  ${v.di} → ${v.den}  ${hm(v.gioDi)} → ${hm(v.gioDen)}</div>
)HTML" +
        R"HTML(    <div class="mut">Ghế <b>${v.ghe}</b>  ${esc(v.hanhKhach.ten)} (${v.hanhKhach.ma})  giá vé gốc ${vnd(v.giaGoc)}</div>${dv ? `<div class="mut">${dv}</div>` : ''}
    ${withHood ? chain(v) : ''}${acts || ''}</div>`;
}
async function pVe() {
  const d = await api('ve'); const ro = user.vaiTro === 'AD';
  $('#panel').innerHTML = `<div class="card"><h2>${ro ? 'Tất cả vé trong hệ thống' : user.vaiTro === 'DL' ? 'Vé đã bán' : 'Vé của tôi'} (${d.ve.length})</h2>` +
    (d.ve.length ? d.ve.map(v => ticketHtml(v, true, ro ? '' : `<div class="acts"><button class="b s" data-c="${v.ma}">Check-in / Đổi chỗ</button>
      <button class="b g s" data-b="${v.ma}">Boarding pass</button><button class="b r s" data-h="${v.ma}">Hủy vé</button></div>`)).join('') : '<div class="empty">Chưa có vé nào</div>') + '</div>';
  $$('[data-c]').forEach(b => b.onclick = guard(() => checkInDlg(d.ve.find(v => v.ma === b.dataset.c))));
  $$('[data-b]').forEach(b => b.onclick = guard(async () => { const r = await api('boardingpass', { maVe: b.dataset.b }); modal(`<h2>Boarding pass ${r.ve.ma}</h2><pre class="bp">${esc(r.text)}</pre><div class="acts"><button class="b" onclick="closeModal()">Đóng</button></div>`); }));
  $$('[data-h]').forEach(b => b.onclick = guard(async () => { if (!confirm('Hủy vé ' + b.dataset.h + '? Ghế sẽ được trả lại.')) return; await post('huyve', { maVe: b.dataset.h }); toast('Đã hủy vé'); await pVe(); }));
}
async function checkInDlg(v) {
  const d = await api('sodoghe', { ma: v.maCB }); let pick = v.ghe;
  const draw = () => { $('#cmap').innerHTML = seatMap(d.ghe.map(g => g.so === v.ghế ? { ...g, tt: 'T' } : g), { sel: pick, cur: v.ghe, hang: v.hang });
    $$('#cmap .seat').forEach(b => b.onclick = () => { pick = b.dataset.so; draw(); $('#pk').textContent = pick; }); };
  modal(`<h2>Check-in vé ${v.ma} — chuyến ${v.maCB}</h2><p class="mut">Ghế hiện tại <b>${v.ghe}</b> (viền cam). Chọn ghế mới cùng hạng ${tagH(v.hang)} hoặc giữ nguyên. Check-in mở trước giờ bay 24 giờ, đóng trước 1 giờ.</p>
    <div id="cmap"></div><p>Ghế đang chọn: <b id="pk">${pick}</b></p>
    
    <div class="acts"><button class="b" id="ok">Xác nhận check-in</button><button class="b g" onclick="closeModal()">Đóng</button></div>`);
  draw();
  $('#ok').onclick = guard(async () => { await post('checkin', { maVe: v.ma, soGhe: pick }); closeModal(); toast('Check-in thành công, ghế ' + pick); await pVe(); });
}

/* ---------------- HÀNH TRÌNH (BUILDER + DECORATOR) ---------------- */
async function pHT() {
  const all = await api('chuyenbay', { sapxep: 'gio' }); S.flights = all.chuyenBay; const dl = user.vaiTro === 'DL';
  $('#panel').innerHTML = `<div class="two"><div class="card"><h2>Đóng hành trình (Builder)</h2>
    <div class="grid"><div><label>Hạng vé</label><select id="hg"><option value="TG">Thương gia (2.5)</option><option value="PT" selected>Phổ thông (1.0)</option><option value="TK">Tiết kiệm (0.8)</option></select></div>
    ${dl ? '<div><label>Mã hành khách</label><input id="mk" value="HK01"></div>' : ''}</div>
    <h3>Các chặng bay</h3><div id="legs"></div>
    <div class="grid" style="margin-top:8px"><div><label>Thêm chặng</label><select id="addsel">${S.flights.map(c => `<option value="${c.ma}">${c.ma}  ${c.di}→${c.den}  ${hm(c.gioDi)}</option>`).join('')}</select></div><div><button class="b g" id="add">+ Thêm chặng</button></div></div>
    <h3>Dịch vụ gia tăng (Decorator)</h3>
    <div class="grid"><div><label>Hành lý thêm (kg)  50.000 /kg</label><input id="kg" type="number" min="0" value="0"></div>
    <div><label>Suất ăn (để trống = không)</label><input id="mon" placeholder="vd: Cơm gà"></div><div><label>Giá suất ăn (VND)</label><input id="gia" type="number" min="0" value="150000"></div></div>
    <div class="acts"><button class="b" id="dat">Đặt hành trình</button></div></div>
    <div class="card"><h2>Kết quả</h2><div id="out"><div class="empty">Chọn các chặng rồi bấm Đặt hành trình</div></div></div></div>`;
  $('#hg').value = S.hanh;
  const drawLegs = () => { $('#legs').innerHTML = S.legs.length ? S.legs.map((m, i) => { const c = S.flights.find(x => x.ma === m); return `<div class="leg"><b>${i + 1}.</b> ${m} ${c ? c.di + ' ? ' + c.den + ' <span class="mut">' + hm(c.gioDi) + '</span>' : ''}<span class="sp"></span><button class="b g s" data-x="${i}">Bỏ</button></div>`; }).join('') : '<div class="mut">Chưa có chặng nào</div>';
    $$('#legs [data-x]').forEach(b => b.onclick = () => { S.legs.splice(b.dataset.x, 1); drawLegs(); }); };
  drawLegs();
  $('#add').onclick = () => { S.legs.push($('#addsel').value); drawLegs(); };
  $('#dat').onclick = guard(async () => {
    S.hanh = $('#hg').value;
    const d = await post('hanhtrinh', { hang: S.hanh, chang: S.legs.join(','), kg: $('#kg').value, mon: $('#mon').value, gia: $('#gia').value, maKhach: dl ? $('#mk').value : '' });
    $('#out').innerHTML = d.ve.map(v => ticketHtml(v, true)).join('') + `<div class="kv"><span>Tổng cộng</span><b class="big">${vnd(d.tong)}</b></div>` +
      hood(['<em>HanhTrinhBuilder</em>.chonHanhKhach().chonHang().themChang()themHanhLy().themSuatAn().<em>build()</em> → <em>HanhTrinh</em>', '<em>QuanLyDatVe::datVeTheoHanhTrinh()</em>: mỗi chặng <em>VeFactory</em> tạo vé, rồi bọc <em>VeCoHanhLy</em> / <em>VeCoSuatAn</em>']);
    S.legs = []; drawLegs(); toast('Đã đặt ' + d.ve.length + ' vé');
  });
}

/* ---------------- THỐNG KÊ ---------------- */
async function pTK() {
  const d = await api('thongke'); const mx = Math.max(1, ...Object.values(d.loaiVe));
  $('#panel').innerHTML = `<div class="stat"><div class="card"><small>Doanh thu vé</small><div class="big">${vnd(d.doanhThu)}</div></div>
    <div class="card"><small>Số vé đã bán</small><div class="big">${d.soVe}</div></div><div class="card"><small>Số chuyến bay</small><div class="big">${d.soChuyen}</div></div>
    ${d.hoaHong !== undefined ? `<div class="card"><small>Hoa hồng đại lý</small><div class="big">${vnd(d.hoaHong)}</div></div>` : ''}</div>
    <div class="card" style="margin-top:16px"><h2>Vé theo hạng</h2>${Object.entries(d.loaiVe).map(([k, n]) => `<div style="display:grid;grid-template-columns:110px 1fr;gap:10px;margin-bottom:8px;align-items:center"><span>${esc(k)}</span><div><div class="bar" style="width:${Math.max(4, n / mx * 100)}%">${n}</div></div></div>`).join('')}
    </div>`;
}

/* ---------------- ADMIN ---------------- */

async function pSS() {
  const all = await api('chuyenbay', { sapxep: 'gio' });
  const dls = await api('daily');
  $('#panel').innerHTML = `<div class="card"><h2>So sánh đại lý & đặt vé hộ</h2>
    <p class="mut">1) Chọn chuyến + hạng vé → So sánh giá &nbsp; 2) Bấm <b>Đặt vé hộ</b> cạnh đại lý → sang giao diện đặt ghế qua đại lý đó.</p>
    <form id="f" class="grid">
      <div><label>Chuyến bay</label><select name="ma">${all.chuyenBay.map(c =>
        `<option value="${c.ma}">${c.ma} ${c.di}→${c.den} ${hm(c.gioDi)} ${vnd(c.gia)}</option>`).join('')}</select></div>
      <div><label>Hạng vé</label><select name="hang">
        <option value="PT">Phổ thông (×1.0)</option>
        <option value="TG">Thương gia (×2.5)</option>
        <option value="TK">Tiết kiệm (×0.8)</option></select></div>
      <div><button class="b" type="submit">So sánh giá</button></div>
    </form>
    <div id="res" style="margin-top:12px"></div></div>`;
  const run = () => {
    const ma = $('#f [name=ma]').value, hang = $('#f [name=hang]').value;
    const c = all.chuyenBay.find(x => x.ma === ma); if (!c) return;
    const heso = hang==='TG'?2.5:hang==='TK'?0.8:1;
    const quay = c.gia * heso;
    let rows = `<tr><td>0</td><td><b>Mua trực tiếp (quầy)</b></td><td>0%</td>
      <td class="right"><b>${vnd(quay)}</b></td>
      <td><button type="button" class="b g" data-dl="" data-ten="Quầy trực tiếp" data-m="${c.ma}">Đặt tại quầy</button></td></tr>`;
    const list = (dls.daiLy||[]).map(d => {
      const gia = quay * (1 - (d.tyLe||0)*0.5);
      return { ...d, gia };
    }).sort((a,b)=>a.gia-b.gia);
    list.forEach((d,i) => {
      rows += `<tr><td>${i+1}</td><td>${esc(d.ten)} <small class="mut">(${d.ma})</small></td>
        <td>${((d.tyLe||0)*100).toFixed(1)}%</td><td class="right">${vnd(d.gia)}</td>
        <td><button type="button" class="b" data-dl="${d.ma}" data-ten="${esc(d.ten)}" data-m="${c.ma}">Đặt vé hộ${i===0?' · rẻ nhất':''}</button></td></tr>`;
    });
    $('#res').innerHTML = `<p>Chuyến <b>${c.ma}</b> ${c.di}→${c.den} — giá cơ bản ${vnd(c.gia)}</p>
      <table><tr><th>STT</th><th>Kênh bán</th><th>HH</th><th class="right">Giá ước tính</th><th>Đặt vé hộ</th></tr>${rows}</table>
      <p class="mut">Sau khi bấm <b>Đặt vé hộ</b>, hệ thống mở màn hình chọn ghế / thanh toán gắn với đại lý tương ứng.</p>`;
    $$('#res [data-m]').forEach(b => b.onclick = () => {
      S.ma = b.dataset.m;
      S.maDaiLy = b.dataset.dl || '';
      S.tenDaiLy = b.dataset.ten || '';
      S.sel = null; S.hold = null;
      S.fromSS = true;
      toast(S.maDaiLy ? ('Chuyển đặt vé hộ qua «' + S.tenDaiLy + '»') : 'Chuyển đặt vé tại quầy');
      go('dat');
    });
  };
  $('#f').onsubmit = e => { e.preventDefault(); run(); }; run();
}

async function pDaily() {
  const d = await api('daily');
  const tk = await api('thongke');
  $('#panel').innerHTML = `<div class="card"><h2>Quản lý đại lý</h2>
    <p class="mut">Doanh thu hệ thống: <b>${vnd(tk.doanhThu||0)}</b> — Tổng vé: <b>${tk.soVe|| (tk.ve?tk.ve:0)}</b></p>
    <p class="mut">Ngày: <b>${(d.daiLy&&d.daiLy[0]&&d.daiLy[0].ngay)||'—'}</b> — HH hôm nay = <b>mức thị trường theo ngày</b> (tăng/giảm so với hôm qua) + <b>bán vé qua ĐL</b> (khi có giao dịch).</p>
    <table><tr><th>Mã</th><th>Tên đại lý</th><th>Tỷ lệ</th><th class="right">HH hôm nay</th><th class="right">Hôm qua</th><th class="right">± so hôm qua</th><th class="right">Tổng HH</th></tr>
    ${(d.daiLy||[]).map(x => {
      const cl = x.chenhLech||0;
      const mau = cl>=0 ? '#059669' : '#dc2626';
      const dau = cl>=0 ? '+' : '';
      return `<tr>
      <td><b>${esc(x.ma)}</b></td><td>${esc(x.ten)}</td>
      <td>${((x.tyLe||0)*100).toFixed(1)}%</td>
      <td class="right">${vnd(x.hoaHongHomNay||0)}<br><small class="mut">TT ${vnd(x.hhThiTruong||0)} + vé ${vnd(x.hhBanVe||0)}</small></td>
      <td class="right">${vnd(x.hoaHongHomQua||0)}</td>
      <td class="right" style="color:${mau};font-weight:600">${dau}${vnd(cl)}</td>
      <td class="right"><b>${vnd(x.tongHoaHong||0)}</b></td></tr>`;
    }).join('')}
    </table>
    <p class="mut">Hoa hồng phản ánh mức độ bán qua từng đại lý — dùng để so sánh cạnh tranh.</p>
  </div>`;
}

async function pQly() {
  const d = await api('chuyenbay', { sapxep: 'gio' });
  const f = (id, title, body) => `<div class="card"><h3 style="margin-top:0">${title}</h3><form id="${id}" class="grid">${body}<div><button class="b">Thêm</button></div></form></div>`;
  $('#panel').innerHTML = `<div class="card"><h2>Danh sách chuyến bay (${d.chuyenBay.length})</h2><div id="tb"></div></div>
    <div class="two">${f('fcb', 'Thêm chuyến bay', `<div><label>Mã chuyến</label><input name="ma" required></div><div><label>Từ</label><select name="di">${apOpts(false)}</select></div><div><label>Đến</label><select name="den">${apOpts(false)}</select></div>
      <div><label>Máy bay</label><select name="mb">${meta.mayBay.map(m => `<option>${esc(m)}</option>`).join('')}</select></div><div><label>Giờ đi</label><input type="datetime-local" name="gioDi" required></div><div><label>Giờ đến</label><input type="datetime-local" name="gioDen" required></div><div><label>Giá cơ bản (VND)</label><input type="number" name="gia" value="1000000" required></div>`)}
    <div>${f('fsb', 'Thêm sân bay', '<div><label>Mã</label><input name="ma" required></div><div><label>Tên</label><input name="ten"></div><div><label>Thành phố</label><input name="tp"></div>')}
    ${f('fmb', 'Thêm máy bay', '<div><label>Số hiệu</label><input name="soHieu" required></div><div><label>Model</label><input name="model"></div><div><label>Ghế TG</label><input type="number" name="tg" value="8"></div><div><label>Ghế PT</label><input type="number" name="pt" value="24"></div><div><label>Ghế TK</label><input type="number" name="tk" value="12"></div>')}</div></div>`;
  $('#tb').innerHTML = flightRows(d.chuyenBay, c => `<button class="b r s" data-d="${c.ma}">Xóa</button>`);
  $$('[data-d]').forEach(b => b.onclick = guard(async () => { if (!confirm('Xóa chuyến ' + b.dataset.d + '?')) return; await post('admin/xoachuyenbay', { ma: b.dataset.d }); toast('Đã xóa'); await pQly(); }));
  const hook = (id, path, msg) => { $('#' + id).onsubmit = guard(async e => { e.preventDefault(); await post(path, Object.fromEntries(new FormData(e.target))); toast(msg); meta = await api('meta'); await pQly(); }); };
  hook('fcb', 'admin/chuyenbay', 'Đã thêm chuyến bay'); hook('fsb', 'admin/sanbay', 'Đã thêm sân bay'); hook('fmb', 'admin/maybay', 'Đã thêm máy bay');
}

(async () => { if (token) await boot(); else loginView(); })();
</script>
</body>
</html>
)HTML";
    return s;
}

using Params = std::map<std::string, std::string>;
const int GIU_CHO_GIAY = 300;   // chi de hien dem nguoc tren giao dien; thoi han that do GiuCho::conHieuLuc() quyet dinh

// ===================== JSON toi gian =====================
std::string esc(const std::string& s) {
    std::string o;
    for (unsigned char c : s) {
        switch (c) {
            case '"': o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\n': o += "\\n"; break;
            case '\r': o += "\\r"; break;
            case '\t': o += "\\t"; break;
            default:
                if (c < 0x20) { char b[8]; std::snprintf(b, sizeof b, "\\u%04x", c); o += b; }
                else o += static_cast<char>(c);
        }
    }
    return o;
}
std::string q(const std::string& s) { return "\"" + esc(s) + "\""; }
std::string num(double v) {
    char b[48];
    if (std::fabs(v - std::floor(v)) < 1e-9 && std::fabs(v) < 1e15) std::snprintf(b, sizeof b, "%lld", static_cast<long long>(v));
    else std::snprintf(b, sizeof b, "%.2f", v);
    return b;
}
std::string mang(const std::vector<std::string>& v) {
    std::string o = "[";
    for (std::size_t i = 0; i < v.size(); ++i) o += (i ? "," : "") + v[i];
    return o + "]";
}
std::string loi(const std::string& m) { return "{\"ok\":false,\"loi\":" + q(m) + "}"; }

// ===================== tien ich =====================
std::string lay(const Params& p, const std::string& k, const std::string& mac = "") {
    auto it = p.find(k);
    return it == p.end() ? mac : it->second;
}
double layThuc(const Params& p, const std::string& k) {
    try { return std::stod(lay(p, k, "0")); } catch (...) { return 0; }
}
std::string cat(std::string s) {
    s.erase(0, s.find_first_not_of(" \t\r\n"));
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
    return s;
}
std::string lamSach(std::string s) {
    for (char& c : s) if (c == ',' || c == ';' || c == '\n' || c == '\r') c = ' ';
    s = cat(s);
    const std::string cach = " + ";
    for (std::size_t vt = s.find(cach); vt != std::string::npos; vt = s.find(cach)) s.replace(vt, cach.size(), " & ");
    return s;
}
std::vector<std::string> tach(const std::string& s, char sep) {
    std::vector<std::string> kq;
    std::stringstream ss(s);
    std::string x;
    while (std::getline(ss, x, sep)) { x = cat(x); if (!x.empty()) kq.push_back(x); }
    return kq;
}
std::string gioStr(time_point tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char b[32];
    std::strftime(b, sizeof b, "%Y-%m-%d %H:%M", &tm);
    return b;
}
// "YYYY-MM-DD" (gio mac dinh 12:00) hoac "YYYY-MM-DDTHH:MM" / "YYYY-MM-DD HH:MM"
bool docGio(const std::string& s, time_point& kq) {
    int y, mo, d, h = 12, mi = 0;
    int n = std::sscanf(s.c_str(), "%d-%d-%dT%d:%d", &y, &mo, &d, &h, &mi);
    if (n < 5) n = std::sscanf(s.c_str(), "%d-%d-%d %d:%d", &y, &mo, &d, &h, &mi);
    if (n < 3) return false;
    if (n < 5) { h = 12; mi = 0; }
    std::tm tm{};
    tm.tm_year = y - 1900; tm.tm_mon = mo - 1; tm.tm_mday = d; tm.tm_hour = h; tm.tm_min = mi; tm.tm_isdst = -1;
    kq = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return true;
}
std::string maHang(HangGhe h) { return h == HangGhe::THUONG_GIA ? "TG" : h == HangGhe::PHO_THONG ? "PT" : "TK"; }
std::string tenHang(HangGhe h) { return h == HangGhe::THUONG_GIA ? "Thương gia" : h == HangGhe::PHO_THONG ? "Phổ thông" : "Tiết kiệm"; }
HangGhe docHang(const std::string& s) {
    if (s == "TG") return HangGhe::THUONG_GIA;
    if (s == "PT") return HangGhe::PHO_THONG;
    if (s == "TK") return HangGhe::TIET_KIEM;
    throw LoiNghiepVu("Hang ve khong hop le (TG/PT/TK)");
}

// ===================== trang thai may chu =====================
struct Giu {
    GiuCho* gc; ChuyenBay* cb; std::string soGhe, chu; DaiLy* dl;
};
// Co the dung bo quan ly noi bo (--web doc lap) hoac gan tu console (dong bo live).
struct TrangThai {
    std::unique_ptr<QuanLyChuyenBay> ownQl;
    std::unique_ptr<QuanLyDatVe> ownDv;
    std::unique_ptr<QuanLyNguoiDung> ownNd;
    std::unique_ptr<FileRepository> ownRepo;
    QuanLyChuyenBay* ql{};
    QuanLyDatVe* dv{};
    QuanLyNguoiDung* nd{};
    FileRepository* repo{};
    std::map<std::string, NguoiDung*> phien;
    std::map<std::string, Giu> giu;
    std::set<std::string> sanBay, mayBay;
    long maxVe = 0;
    int demGiu = 0;
    std::mutex mtx;
    std::atomic<bool> dangChay{false};
    sock_t listenSock = INVALID_SOCKET;

    void dungNoiBo() {
        ownQl = std::make_unique<QuanLyChuyenBay>();
        ownDv = std::make_unique<QuanLyDatVe>(*ownQl);
        ownNd = std::make_unique<QuanLyNguoiDung>();
        ownRepo = std::make_unique<FileRepository>("data");
        ql = ownQl.get(); dv = ownDv.get(); nd = ownNd.get(); repo = ownRepo.get();
    }
    void ganTuConsole(QuanLyChuyenBay& eql, QuanLyDatVe& edv, QuanLyNguoiDung& end, FileRepository& erepo) {
        ownQl.reset(); ownDv.reset(); ownNd.reset(); ownRepo.reset();
        ql = &eql; dv = &edv; nd = &end; repo = &erepo;
    }
} S;

void luu() {
    try { S.repo->ghiNguoiDung(*S.nd); S.repo->ghiChuyenBay(*S.ql); S.repo->ghiVe(*S.dv); }
    catch (const std::exception& e) { std::cerr << "[canh bao] khong luu duoc: " << e.what() << "\n"; }
}

std::string maVeTu(long i) { char b[24]; std::snprintf(b, sizeof b, "VE%05ld", i); return b; }
long soVe(const std::string& ma) {
    try { return ma.size() > 2 ? std::stol(ma.substr(2)) : 0; } catch (...) { return 0; }
}
// UML khong co ham liet ke ve -> do ma VE00001..VE<maxVe> bang timVe() (luon dong bo voi du lieu that)
std::vector<VeMayBay*> tatCaVe() {
    std::vector<VeMayBay*> kq;
    for (long i = 1; i <= S.maxVe; ++i) if (VeMayBay* v = S.dv->timVe(maVeTu(i))) kq.push_back(v);
    return kq;
}
void ghiNhanVe(const VeMayBay* v) { S.maxVe = std::max(S.maxVe, soVe(v->getMaVe())); }

void biet(const ChuyenBay* cb) {
    S.sanBay.insert(cb->getSanBayDi()->getMaSB());
    S.sanBay.insert(cb->getSanBayDen()->getMaSB());
    S.mayBay.insert(cb->getMayBay()->getSoHieu());
}

// ===================== JSON cho doi tuong =====================
std::string chuyenJson(ChuyenBay* cb) {
    cb->donDepGiuCho();
    SoDoGhe& sd = cb->getSoDoGhe();
    MayBay* mb = cb->getMayBay();
    const SanBay* di = cb->getSanBayDi();
    const SanBay* den = cb->getSanBayDen();
    auto bo = [&](HangGhe h) { return "{\"trong\":" + std::to_string(sd.demGheTrong(h)) + ",\"tong\":" + std::to_string(mb->getSoGhe(h)) + "}"; };
    return "{\"ma\":" + q(cb->getMaCB()) + ",\"di\":" + q(di->getMaSB()) + ",\"diTen\":" + q(di->getThanhPho()) +
           ",\"den\":" + q(den->getMaSB()) + ",\"denTen\":" + q(den->getThanhPho()) +
           ",\"gioDi\":" + q(gioStr(cb->getGioDi())) + ",\"gioDen\":" + q(gioStr(cb->getGioDen())) +
           ",\"phut\":" + std::to_string(cb->thoiGianBay().count()) + ",\"gia\":" + num(cb->getGiaCoBan()) +
           ",\"mayBay\":" + q(mb->getSoHieu()) + ",\"TG\":" + bo(HangGhe::THUONG_GIA) + ",\"PT\":" + bo(HangGhe::PHO_THONG) +
           ",\"TK\":" + bo(HangGhe::TIET_KIEM) + "}";
}

std::vector<std::string> dichVuTu(const std::string& moTa) {   // "ma - hang + dv1 + dv2" -> {dv1, dv2}
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

std::string veJson(const VeMayBay* v) {
    const ChuyenBay* cb = v->getChuyenBay();
    const VeMayBay* goc = v->veGoc();
    std::string lopGoc = dynamic_cast<const VeThuongGia*>(goc) ? "VeThuongGia" : dynamic_cast<const VePhoThong*>(goc) ? "VePhoThong" : "VeTietKiem";
    std::string hang = lopGoc == "VeThuongGia" ? "TG" : lopGoc == "VePhoThong" ? "PT" : "TK";
    std::vector<std::string> dv = dichVuTu(v->moTa()), dvJson;
    std::string doiTuong = lopGoc;                       // vd VeCoSuatAn(VeCoHanhLy(VePhoThong))
    for (const auto& d : dv) {
        doiTuong = std::string(d.rfind("Hanh ly", 0) == 0 ? "VeCoHanhLy" : "VeCoSuatAn") + "(" + doiTuong + ")";
        dvJson.push_back(q(d));
    }
    const HanhKhach* hk = v->getHanhKhach();
    return "{\"ma\":" + q(v->getMaVe()) + ",\"loai\":" + q(v->loaiVe()) + ",\"hang\":" + q(hang) +
           ",\"moTa\":" + q(v->moTa()) + ",\"ghe\":" + q(v->getSoGhe()) + ",\"maCB\":" + q(cb->getMaCB()) +
           ",\"di\":" + q(cb->getSanBayDi()->getMaSB()) + ",\"den\":" + q(cb->getSanBayDen()->getMaSB()) +
           ",\"gioDi\":" + q(gioStr(cb->getGioDi())) + ",\"gioDen\":" + q(gioStr(cb->getGioDen())) +
           ",\"gia\":" + num(v->tinhGia()) + ",\"giaGoc\":" + num(goc->tinhGia()) +
           ",\"checkIn\":" + (v->isDaCheckIn() ? "true" : "false") + ",\"doiTuong\":" + q(doiTuong) +
           ",\"dichVu\":" + mang(dvJson) + ",\"hanhKhach\":{\"ma\":" + q(hk->getMaND()) + ",\"ten\":" + q(hk->getHoTen()) + "}}";
}

std::string nguoiJson(NguoiDung* u) {
    const char* vt = dynamic_cast<Admin*>(u) ? "AD" : dynamic_cast<DaiLy*>(u) ? "DL" : "HK";
    std::string s = "{\"ma\":" + q(u->getMaND()) + ",\"ten\":" + q(u->getHoTen()) + ",\"vaiTro\":" + q(vt) +
                    ",\"vaiTroTen\":" + q(u->vaiTro());
    if (auto* dl = dynamic_cast<DaiLy*>(u)) s += ",\"hoaHong\":" + num(dl->getTongHoaHong());
    return s + "}";
}

// ===================== xac thuc =====================
NguoiDung* nguoiDung(const Params& p) {
    auto it = S.phien.find(lay(p, "token"));
    if (it == S.phien.end()) throw LoiNghiepVu("Chua dang nhap hoac phien da het han");
    return it->second;
}
void canQuyen(NguoiDung* u, const std::string& ds) {   // ds: vd "AD" hoac "HKDL"
    const char* vt = dynamic_cast<Admin*>(u) ? "AD" : dynamic_cast<DaiLy*>(u) ? "DL" : "HK";
    if (ds.find(vt) == std::string::npos) throw LoiNghiepVu("Vai tro cua ban khong duoc phep thuc hien thao tac nay");
}
VeMayBay* veCuaToi(NguoiDung* u, const std::string& ma) {
    VeMayBay* v = S.dv->timVe(ma);
    if (!v) throw LoiNghiepVu("Khong tim thay ve " + ma);
    if (auto* hk = dynamic_cast<HanhKhach*>(u)) if (v->getHanhKhach() != hk) throw LoiNghiepVu("Ve nay khong phai cua ban");
    return v;
}
// Hanh khach tu dat; dai ly dat ho (maKhach); admin khong duoc dat ve.
void nguoiMua(NguoiDung* u, const Params& p, HanhKhach*& hk, DaiLy*& dl) {
    hk = nullptr; dl = nullptr;
    if (auto* h = dynamic_cast<HanhKhach*>(u)) { hk = h; return; }
    if (auto* d = dynamic_cast<DaiLy*>(u)) {
        dl = d;
        const std::string ma = lay(p, "maKhach");
        hk = dynamic_cast<HanhKhach*>(S.nd->timNguoiDung(ma));
        if (!hk) throw LoiNghiepVu("Khong tim thay hanh khach \"" + ma + "\" (vd HK01)");
        return;
    }
    throw LoiNghiepVu("Admin khong dat ve");
}

// ===================== cac API =====================
std::string hLogin(const Params& p) {
    NguoiDung* u = S.nd->dangNhap(lay(p, "ma"), lay(p, "mk"));
    if (!u) throw LoiNghiepVu("Sai ma nguoi dung hoac mat khau");
    std::random_device rd;
    char tk[40];
    std::snprintf(tk, sizeof tk, "%08x%08x%08x", rd(), rd(), rd());
    S.phien[tk] = u;
    return "{\"ok\":true,\"token\":" + q(tk) + ",\"user\":" + nguoiJson(u) + "}";
}
std::string hDangKy(const Params& p) {
    // Chi cho dang ky Hanh khach - Admin/Dai ly khong tu tao qua web
    const std::string ma = lamSach(lay(p, "ma"));
    const std::string mk = lay(p, "mk");
    const std::string ten = lamSach(lay(p, "ten"));
    const std::string cccd = lamSach(lay(p, "cccd"));
    const std::string sdt = lamSach(lay(p, "sdt"));
    const std::string email = lamSach(lay(p, "email"));
    // Gom tat ca loi de bao cung luc (co dau)
    std::vector<std::string> loi;
    if (ma.empty())
        loi.push_back("Mã người dùng bắt buộc (vd HK01)");
    else if (ma.rfind("AD", 0) == 0 || ma.rfind("DL", 0) == 0)
        loi.push_back("Không được dùng mã AD/DL. Chỉ đăng ký Hành khách (vd HK03)");
    else if (S.nd->timNguoiDung(ma))
        loi.push_back("Mã người dùng đã tồn tại, chọn mã khác");
    if (ten.empty())
        loi.push_back("Họ tên bắt buộc");
    if (mk.size() < 3)
        loi.push_back("Mật khẩu ít nhất 3 ký tự");
    if (cccd.empty())
        loi.push_back("CCCD bắt buộc");
    else if (!((cccd.size() == 9 || cccd.size() == 12) && chiChuSo(cccd)))
        loi.push_back("CCCD không hợp lệ — phải 9 hoặc 12 chữ số (vd 001234567890)");
    if (sdt.empty())
        loi.push_back("Số điện thoại bắt buộc");
    else if (!((sdt.size() == 10 || sdt.size() == 11) && chiChuSo(sdt) && sdt[0] == '0'))
        loi.push_back("Số điện thoại không hợp lệ — phải 10–11 chữ số, bắt đầu bằng 0 (vd 0901234567)");
    if (email.empty())
        loi.push_back("Email bắt buộc");
    else if (!emailHopLe(email))
        loi.push_back("Email không hợp lệ — cần có @ và tên miền (vd ten@gmail.com)");
    if (!loi.empty()) {
        std::string msg = "Có " + std::to_string(loi.size()) + " lỗi:\n";
        for (std::size_t i = 0; i < loi.size(); ++i)
            msg += "• " + loi[i] + (i + 1 < loi.size() ? "\n" : "");
        throw LoiNghiepVu(msg);
    }
    S.nd->themNguoiDung(std::make_unique<HanhKhach>(ma, ten, mk, cccd, sdt, email));
    try {
        S.repo->ghiNguoiDung(*S.nd);
    } catch (const std::exception& e) {
        throw LoiNghiepVu(std::string("Đăng ký xong nhưng không ghi được file: ") + e.what()
            + " — chạy datve.exe trong thư mục có folder data/");
    }
    return "{\"ok\":true,\"thongBao\":\"Đăng ký thành công. Dùng mã này đăng nhập console/web.\",\"ma\":" + q(ma) + "}";
}

std::string hMe(const Params& p) { return "{\"ok\":true,\"user\":" + nguoiJson(nguoiDung(p)) + "}"; }
std::string hLogout(const Params& p) { S.phien.erase(lay(p, "token")); return "{\"ok\":true}"; }

std::string hMeta(const Params&) {
    std::vector<std::string> sb, mb;
    for (const auto& ma : S.sanBay) if (SanBay* s = S.ql->timSanBay(ma))
        sb.push_back("{\"ma\":" + q(ma) + ",\"ten\":" + q(s->getTen()) + ",\"tp\":" + q(s->getThanhPho()) + "}");
    for (const auto& ma : S.mayBay) mb.push_back(q(ma));
    return "{\"ok\":true,\"sanBay\":" + mang(sb) + ",\"mayBay\":" + mang(mb) + "}";
}

std::string hChuyenBay(const Params& p) {
    time_point ngay{};
    const std::string n = lay(p, "ngay");
    if (!n.empty() && !docGio(n, ngay)) throw LoiNghiepVu("Ngay khong hop le (YYYY-MM-DD)");
    std::vector<ChuyenBay*> ds = S.ql->timChuyenBay(lay(p, "di"), lay(p, "den"), ngay);
    if (lay(p, "sapxep") == "gia") S.ql->sapXepTheoGia(ds); else S.ql->sapXepTheoGio(ds);
    std::vector<std::string> out;
    for (ChuyenBay* cb : ds) out.push_back(chuyenJson(cb));
    return "{\"ok\":true,\"chuyenBay\":" + mang(out) + "}";
}

std::string hNoiChuyen(const Params& p) {
    time_point ngay{};
    const std::string n = lay(p, "ngay");
    if (!n.empty() && !docGio(n, ngay)) throw LoiNghiepVu("Ngay khong hop le (YYYY-MM-DD)");
    const int cho = static_cast<int>(layThuc(p, "cho") > 0 ? layThuc(p, "cho") : 60);
    const int toiDa = static_cast<int>(layThuc(p, "toida") > 0 ? layThuc(p, "toida") : 3);
    auto kq = S.ql->timNoiChuyen(lay(p, "di"), lay(p, "den"), ngay, std::chrono::minutes(cho), toiDa);
    std::vector<std::string> out;
    for (const auto& duong : kq) {
        std::vector<std::string> chang;
        double tong = 0;
        for (ChuyenBay* cb : duong) { chang.push_back(chuyenJson(cb)); tong += cb->getGiaCoBan(); }
        const long long phut = std::chrono::duration_cast<std::chrono::minutes>(duong.back()->getGioDen() - duong.front()->getGioDi()).count();
        out.push_back("{\"chang\":" + mang(chang) + ",\"tongGia\":" + num(tong) + ",\"tongPhut\":" + std::to_string(phut) + "}");
    }
    return "{\"ok\":true,\"hanhTrinh\":" + mang(out) + "}";
}

// UML khong co ham liet ke ghe -> do cac nhan ghe co the co (hang 1..80, cot A..F) bang timGhe()
std::string hSoDoGhe(const Params& p) {
    ChuyenBay* cb = S.ql->timChuyenBay(lay(p, "ma"));
    if (!cb) throw LoiNghiepVu("Khong co chuyen bay " + lay(p, "ma"));
    cb->donDepGiuCho();
    std::vector<std::string> ghe;
    for (int h = 1; h <= 80; ++h)
        for (char c = 'A'; c <= 'F'; ++c) {
            const std::string nhan = std::to_string(h) + c;
            Ghe* g = cb->getSoDoGhe().timGhe(nhan);
            if (!g) continue;
            const char* tt = g->getTrangThai() == TrangThaiGhe::TRONG ? "T" : g->getTrangThai() == TrangThaiGhe::GIU_CHO ? "H" : "X";
            ghe.push_back("{\"so\":" + q(nhan) + ",\"hang\":" + q(maHang(g->getHang())) + ",\"tt\":" + q(tt) + "}");
        }
    return "{\"ok\":true,\"chuyenBay\":" + chuyenJson(cb) + ",\"ghe\":" + mang(ghe) + "}";
}

std::string hGiuCho(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    HanhKhach* hk; DaiLy* dl;
    nguoiMua(u, p, hk, dl);
    // Hanh khach co the chon mua qua dai ly (maDaiLy tu man so sanh)
    if (!dl) {
        const std::string maDl = lay(p, "maDaiLy");
        if (!maDl.empty()) {
            dl = dynamic_cast<DaiLy*>(S.nd->timNguoiDung(maDl));
            if (!dl) throw LoiNghiepVu("Khong tim thay dai ly " + maDl);
        }
    }
    // Tuy chon "giay" (5..299): demo het han nhanh - goi thang ChuyenBay::giuGhe voi timeout ngan.
    const int giay = static_cast<int>(layThuc(p, "giay"));
    GiuCho* gc = nullptr;
    if (giay >= 5 && giay < GIU_CHO_GIAY) {
        ChuyenBay* cb = S.ql->timChuyenBay(lay(p, "maCB"));
        if (!cb) throw LoiNghiepVu("Khong co chuyen bay " + lay(p, "maCB"));
        gc = cb->giuGhe(lay(p, "soGhe"), hk, std::chrono::seconds(giay));
    } else {
        gc = S.dv->giuCho(lay(p, "maCB"), lay(p, "soGhe"), hk);
    }
    const int conLai = (giay >= 5 && giay < GIU_CHO_GIAY) ? giay : GIU_CHO_GIAY;
    const std::string id = "G" + std::to_string(++S.demGiu);
    S.giu[id] = Giu{gc, gc->getChuyenBay(), gc->getSoGhe(), u->getMaND(), dl};
    const Ghe* g = gc->getChuyenBay()->getSoDoGhe().timGhe(gc->getSoGhe());
    return "{\"ok\":true,\"holdId\":" + q(id) + ",\"conLai\":" + std::to_string(conLai) + ",\"maCB\":" + q(gc->getChuyenBay()->getMaCB()) +
           ",\"soGhe\":" + q(gc->getSoGhe()) + ",\"hang\":" + q(maHang(g->getHang())) + ",\"hangTen\":" + q(tenHang(g->getHang())) + "}";
}

std::map<std::string, Giu>::iterator timGiu(NguoiDung* u, const Params& p) {
    auto it = S.giu.find(lay(p, "holdId"));
    if (it == S.giu.end()) throw LoiNghiepVu("Khong tim thay phien giu cho (co the da het han hoac da xu ly)");
    if (it->second.chu != u->getMaND()) throw LoiNghiepVu("Phien giu cho nay khong phai cua ban");
    return it;
}

std::string hHuyGiuCho(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    auto it = timGiu(u, p);
    it->second.cb->huyGiu(it->second.soGhe);
    S.giu.erase(it);
    return "{\"ok\":true}";
}

std::string hThanhToan(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    auto it = timGiu(u, p);
    Giu g = it->second;
    S.giu.erase(it);                                            // phien chi dung duoc 1 lan
    const Ghe* ghe = g.cb->getSoDoGhe().timGhe(g.soGhe);
    VeMayBay* ve = S.dv->xacNhanThanhToan(g.gc, ghe->getHang(), g.dl);   // nem LoiGiuChoHetHan neu qua han
    ghiNhanVe(ve);
    luu();
    return "{\"ok\":true,\"ve\":" + veJson(ve) + "}";
}

std::string hHanhTrinh(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    HanhKhach* hk; DaiLy* dl;
    nguoiMua(u, p, hk, dl);
    HanhTrinhBuilder b;
    b.chonHanhKhach(hk).chonHang(docHang(lay(p, "hang")));
    const std::vector<std::string> cacChang = tach(lay(p, "chang"), ',');
    if (cacChang.empty()) throw LoiNghiepVu("Chua chon chang bay nao");
    for (const auto& ma : cacChang) {
        ChuyenBay* cb = S.ql->timChuyenBay(ma);
        if (!cb) throw LoiNghiepVu("Khong co chuyen bay " + ma);
        b.themChang(cb);
    }
    if (layThuc(p, "kg") > 0) b.themHanhLy(layThuc(p, "kg"));
    const std::string mon = lamSach(lay(p, "mon"));
    if (!mon.empty()) b.themSuatAn(mon, layThuc(p, "gia"));
    const HanhTrinh ht = b.build();
    const std::vector<VeMayBay*> ds = S.dv->datVeTheoHanhTrinh(ht, dl);
    std::vector<std::string> out;
    double tong = 0;
    for (const VeMayBay* v : ds) { ghiNhanVe(v); out.push_back(veJson(v)); tong += v->tinhGia(); }
    luu();
    return "{\"ok\":true,\"ve\":" + mang(out) + ",\"tong\":" + num(tong) + "}";
}

std::string hVe(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    std::vector<std::string> out;
    for (const VeMayBay* v : tatCaVe()) {
        if (auto* hk = dynamic_cast<HanhKhach*>(u)) if (v->getHanhKhach() != hk) continue;
        out.push_back(veJson(v));
    }
    return "{\"ok\":true,\"ve\":" + mang(out) + "}";
}

std::string hCheckIn(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    canQuyen(u, "HKDL");
    VeMayBay* v = veCuaToi(u, lay(p, "maVe"));
    const std::string ghe = lay(p, "soGhe").empty() ? v->getSoGhe() : lay(p, "soGhe");
    S.dv->checkIn(v->getMaVe(), ghe);
    luu();
    return "{\"ok\":true,\"ve\":" + veJson(v) + "}";
}

std::string hBoardingPass(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    VeMayBay* v = veCuaToi(u, lay(p, "maVe"));
    std::ostringstream cap;                                     // bat dau ra cua VeMayBay::inBoardingPass() de hien len web
    std::streambuf* cu = std::cout.rdbuf(cap.rdbuf());
    try { S.dv->inBoardingPass(v->getMaVe()); } catch (...) { std::cout.rdbuf(cu); throw; }
    std::cout.rdbuf(cu);
    return "{\"ok\":true,\"text\":" + q(cap.str()) + ",\"ve\":" + veJson(v) + "}";
}

std::string hHuyVe(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    canQuyen(u, "HKDL");
    VeMayBay* v = veCuaToi(u, lay(p, "maVe"));
    S.dv->huyVe(v->getMaVe());
    luu();
    return "{\"ok\":true}";
}


std::string hDaiLy(const Params&) {
    std::vector<std::string> out;
    for (NguoiDung* u : S.nd->layDsNguoiDung()) {
        auto* dl = dynamic_cast<DaiLy*>(u);
        if (!dl) continue;
        dl->dongBoNgayThuc();
        out.push_back("{\"ma\":" + q(dl->getMaND()) + ",\"ten\":" + q(dl->getHoTen())
            + ",\"tyLe\":" + num(dl->getTyLeHoaHong())
            + ",\"tongHoaHong\":" + num(dl->getTongHoaHong())
            + ",\"hoaHongHomNay\":" + num(dl->getHoaHongHomNay())
            + ",\"hoaHongHomQua\":" + num(dl->getHoaHongHomQua())
            + ",\"chenhLech\":" + num(dl->getChenhLechVoiHomQua())
            + ",\"hhBanVe\":" + num(dl->getHoaHongBanVeHomNay())
            + ",\"hhThiTruong\":" + num(dl->getHoaHongThiTruongHomNay())
            + ",\"ngay\":" + q(dl->getNgayHoaHong()) + "}");
    }
    return "{\"ok\":true,\"daiLy\":" + mang(out) + "}";
}

std::string hThongKe(const Params& p) {
    NguoiDung* u = nguoiDung(p);
    canQuyen(u, "ADDL");
    std::map<std::string, int> loai = S.dv->thongKeLoaiVe();
    const std::vector<VeMayBay*> ve = tatCaVe();
    std::string s = "{\"ok\":true,\"doanhThu\":" + num(S.dv->tinhDoanhThu()) + ",\"soVe\":" + std::to_string(ve.size()) +
                    ",\"soChuyen\":" + std::to_string(S.ql->timChuyenBay("", "", time_point{}).size()) + ",\"loaiVe\":{";
    bool dau = true;
    for (const auto& kv : loai) { s += (dau ? "" : ",") + q(kv.first) + ":" + std::to_string(kv.second); dau = false; }
    s += "}";
    if (auto* dl = dynamic_cast<DaiLy*>(u)) s += ",\"hoaHong\":" + num(dl->getTongHoaHong());
    return s + "}";
}

std::string hThemSanBay(const Params& p) {
    canQuyen(nguoiDung(p), "AD");
    const std::string ma = lamSach(lay(p, "ma"));
    if (ma.empty()) throw LoiNghiepVu("Ma san bay khong duoc de trong");
    S.ql->themSanBay(std::make_unique<SanBay>(ma, lamSach(lay(p, "ten")), lamSach(lay(p, "tp"))));
    S.sanBay.insert(ma);
    luu();
    return "{\"ok\":true}";
}
std::string hThemMayBay(const Params& p) {
    canQuyen(nguoiDung(p), "AD");
    const std::string ma = lamSach(lay(p, "soHieu"));
    const int tg = static_cast<int>(layThuc(p, "tg")), pt = static_cast<int>(layThuc(p, "pt")), tk = static_cast<int>(layThuc(p, "tk"));
    if (ma.empty() || tg < 0 || pt < 0 || tk < 0 || tg + pt + tk == 0) throw LoiNghiepVu("Thong tin may bay khong hop le");
    S.ql->themMayBay(std::make_unique<MayBay>(ma, lamSach(lay(p, "model")), tg, pt, tk));
    S.mayBay.insert(ma);
    luu();
    return "{\"ok\":true}";
}
std::string hThemChuyenBay(const Params& p) {
    canQuyen(nguoiDung(p), "AD");
    const std::string ma = lamSach(lay(p, "ma"));
    SanBay* di = S.ql->timSanBay(lay(p, "di"));
    SanBay* den = S.ql->timSanBay(lay(p, "den"));
    MayBay* mb = S.ql->timMayBay(lay(p, "mb"));
    time_point gioDi, gioDen;
    if (ma.empty() || !di || !den || !mb || di == den) throw LoiNghiepVu("Ma chuyen bay / san bay / may bay khong hop le");
    if (!docGio(lay(p, "gioDi"), gioDi) || !docGio(lay(p, "gioDen"), gioDen)) throw LoiNghiepVu("Dinh dang gio khong dung");
    if (gioDen <= gioDi) throw LoiNghiepVu("Giờ đến phai sau gio di");
    if (layThuc(p, "gia") <= 0) throw LoiNghiepVu("Gia phai lon hon 0");
    S.ql->themChuyenBay(std::make_unique<ChuyenBay>(ma, di, den, mb, gioDi, gioDen, layThuc(p, "gia")));
    luu();
    return "{\"ok\":true}";
}
std::string hXoaChuyenBay(const Params& p) {
    canQuyen(nguoiDung(p), "AD");
    ChuyenBay* cb = S.ql->timChuyenBay(lay(p, "ma"));
    S.ql->xoaChuyenBay(lay(p, "ma"));
    for (auto it = S.giu.begin(); it != S.giu.end();) it = (it->second.cb == cb) ? S.giu.erase(it) : std::next(it);
    luu();
    return "{\"ok\":true}";
}

typedef std::string (*Handler)(const Params&);
const std::map<std::string, Handler> TUYEN = {
    {"POST /api/login", hLogin}, {"POST /api/dangky", hDangKy},           {"GET /api/me", hMe},                  {"POST /api/logout", hLogout},
    {"GET /api/meta", hMeta},              {"GET /api/chuyenbay", hChuyenBay},    {"GET /api/noichuyen", hNoiChuyen},
    {"GET /api/sodoghe", hSoDoGhe},        {"POST /api/giucho", hGiuCho},         {"POST /api/huygiucho", hHuyGiuCho},
    {"POST /api/thanhtoan", hThanhToan},   {"POST /api/hanhtrinh", hHanhTrinh},   {"GET /api/ve", hVe},
    {"POST /api/checkin", hCheckIn},       {"GET /api/boardingpass", hBoardingPass}, {"POST /api/huyve", hHuyVe},
    {"GET /api/thongke", hThongKe}, {"GET /api/daily", hDaiLy},        {"POST /api/admin/sanbay", hThemSanBay}, {"POST /api/admin/maybay", hThemMayBay},
    {"POST /api/admin/chuyenbay", hThemChuyenBay}, {"POST /api/admin/xoachuyenbay", hXoaChuyenBay}};

// ===================== HTTP =====================

std::string giaiMa(const std::string& s) {
    std::string o;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '+') o += ' ';
        else if (s[i] == '%' && i + 2 < s.size() + 0 && std::isxdigit(static_cast<unsigned char>(s[i + 1])) && std::isxdigit(static_cast<unsigned char>(s[i + 2]))) {
            o += static_cast<char>(std::stoi(s.substr(i + 1, 2), nullptr, 16));
            i += 2;
        } else o += s[i];
    }
    return o;
}
void docThamSo(const std::string& s, Params& p) {
    std::stringstream ss(s);
    std::string kv;
    while (std::getline(ss, kv, '&')) {
        const std::size_t e = kv.find('=');
        if (e == std::string::npos) { if (!kv.empty()) p[giaiMa(kv)] = ""; }
        else p[giaiMa(kv.substr(0, e))] = giaiMa(kv.substr(e + 1));
    }
}
std::string dap(int ma, const std::string& loai, const std::string& body) {
    const char* tt = ma == 200 ? "OK" : ma == 404 ? "Not Found" : ma == 400 ? "Bad Request" : "Error";
    return "HTTP/1.1 " + std::to_string(ma) + " " + tt + "\r\nContent-Type: " + loai + "; charset=utf-8\r\nContent-Length: " +
           std::to_string(body.size()) + "\r\nCache-Control: no-store\r\nConnection: close\r\n\r\n" + body;
}
void guiHet(sock_t c, const std::string& s) {
    std::size_t da = 0;
    while (da < s.size()) {
        const int n = static_cast<int>(::send(c, s.data() + da, static_cast<int>(s.size() - da), 0));
        if (n <= 0) return;
        da += static_cast<std::size_t>(n);
    }
}
void phucVu(sock_t c) {
#ifdef _WIN32
    DWORD ms = 1000;
    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&ms), sizeof ms);
#else
    timeval tv{1, 0};
    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
#endif
    std::string buf;
    char tmp[4096];
    std::size_t dauHet = std::string::npos;
    while (dauHet == std::string::npos && buf.size() < 65536) {
        const int n = static_cast<int>(::recv(c, tmp, sizeof tmp, 0));
        if (n <= 0) { DONG_SOCK(c); return; }                    // ket noi nhan roi / het gio cho
        buf.append(tmp, static_cast<std::size_t>(n));
        dauHet = buf.find("\r\n\r\n");
    }
    if (dauHet == std::string::npos) { DONG_SOCK(c); return; }
    const std::string head = buf.substr(0, dauHet);
    std::string body = buf.substr(dauHet + 4);
    std::size_t dai = 0;
    {
        std::string thap = head;
        for (char& ch : thap) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        const std::size_t vt = thap.find("content-length:");
        if (vt != std::string::npos) dai = static_cast<std::size_t>(std::atol(thap.c_str() + vt + 15));
    }
    while (body.size() < dai && body.size() < 1048576) {
        const int n = static_cast<int>(::recv(c, tmp, sizeof tmp, 0));
        if (n <= 0) break;
        body.append(tmp, static_cast<std::size_t>(n));
    }
    std::istringstream rl(head.substr(0, head.find("\r\n")));
    std::string phuongThuc, dich, giaoThuc;
    rl >> phuongThuc >> dich >> giaoThuc;
    std::string duong = dich, truyVan;
    const std::size_t h = dich.find('?');
    if (h != std::string::npos) { duong = dich.substr(0, h); truyVan = dich.substr(h + 1); }
    Params p;
    docThamSo(truyVan, p);
    if (phuongThuc == "POST") docThamSo(body, p);

    std::string kq;
    {
        if (duong.rfind("/api/", 0) == 0) {
            auto it = TUYEN.find(phuongThuc + " " + duong);
            if (it == TUYEN.end()) kq = dap(404, "application/json", loi("Khong co API " + phuongThuc + " " + duong));
            else {
                try { kq = dap(200, "application/json", it->second(p)); }
                catch (const std::exception& e) { kq = dap(200, "application/json", loi(e.what())); }
            }
        } else if (duong == "/" || duong == "/index.html") {
            kq = dap(200, "text/html", trangChu());
        } else if (duong == "/favicon.ico") {
            kq = "HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n";
        } else {
            kq = dap(404, "text/plain", "404");
        }
    }
    guiHet(c, kq);
    DONG_SOCK(c);
}

void vongLapChapNhan();
bool batDauTuConsole(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, FileRepository& repo, int cong = 8080);

int chayWeb(int argc, char** argv) {
    batUtf8Console();
    int cong = 8080;
    std::string host = "127.0.0.1";
    bool datLai = false;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--port" && i + 1 < argc) cong = std::atoi(argv[++i]);
        else if (a == "--host" && i + 1 < argc) host = argv[++i];
        else if (a == "--reset") datLai = true;
        else if (a == "--web") {}
        else { std::cout << "Dung: chuong_trinh --web [--port 8080] [--host 127.0.0.1|0.0.0.0] [--reset]\n"; return 0; }
    }
    if (datLai) std::filesystem::remove_all("data");
    S.dungNoiBo();
    try {
        if (std::filesystem::exists("data/nguoidung.csv")) {
            S.repo->docNguoiDung(*S.nd);
            S.repo->docChuyenBay(*S.ql);
            S.repo->docVe(*S.dv);
            std::cout << "Đã nạp dữ liệu từ thư mục data/ (dung --reset de bat dau lai tu du lieu mau)\n";
            capNhatGioChuyenTheoThoiGianThuc(*S.ql);
        } else {
            nhapDuLieuMau(*S.ql, *S.nd);
            luu();
            std::cout << "Lần chạy đầu: đã tạo dữ liệu mẫu.\n";
        }
        for (ChuyenBay* cb : S.ql->timChuyenBay("", "", time_point{})) biet(cb);
        for (long i = 1; i <= 5000; ++i) if (VeMayBay* v = S.dv->timVe(maVeTu(i))) ghiNhanVe(v);
    } catch (const std::exception& e) {
        std::cerr << "Loi nap du lieu: " << e.what() << "\n";
        return 1;
    }
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    sock_t ls = ::socket(AF_INET, SOCK_STREAM, 0);
    if (ls == INVALID_SOCKET) { std::cerr << "Khong tao duoc socket\n"; return 1; }
    int bat = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&bat), sizeof bat);
    sockaddr_in dc{};
    dc.sin_family = AF_INET;
    dc.sin_port = htons(static_cast<unsigned short>(cong));
    if (inet_pton(AF_INET, host.c_str(), &dc.sin_addr) != 1) { std::cerr << "Dia chi khong hop le: " << host << "\n"; return 1; }
    if (::bind(ls, reinterpret_cast<sockaddr*>(&dc), sizeof dc) != 0 || ::listen(ls, 64) != 0) {
        std::cerr << "Khong mo duoc cong " << cong << " (co the dang bi chuong trinh khac dung). Thu: --port 8081\n";
        return 1;
    }
    S.listenSock = ls;
    S.dangChay = true;
    std::cout << "\n  WEB DEMO dang chay:  http://" << (host == "0.0.0.0" ? "localhost" : host) << ":" << cong << "\n"
              << "  Tai khoan: AD01/admin123  DL01/daily123  HK01/hk123  HK02/hk456\n"
              << "  Nhan Ctrl+C de dung.\n\n";
    vongLapChapNhan();
    return 0;
}

void vongLapChapNhan() {
    while (S.dangChay.load()) {
        sock_t c = ::accept(S.listenSock, nullptr, nullptr);
        if (c == INVALID_SOCKET) continue;
        {
            std::lock_guard<std::mutex> lock(S.mtx);
            phucVu(c);
        }
    }
    if (S.listenSock != INVALID_SOCKET) {
        DONG_SOCK(S.listenSock);
        S.listenSock = INVALID_SOCKET;
    }
}

// Goi tu menu console: dung CHUNG bo quan ly -> moi thay doi console hien ngay tren web.

std::vector<std::string> layDiaChiLan() {
    std::vector<std::string> out;
    char host[256]{};
    if (gethostname(host, sizeof host) != 0) return out;
    addrinfo goi{}, *res = nullptr;
    goi.ai_family = AF_INET;
    if (getaddrinfo(host, nullptr, &goi, &res) != 0) return out;
    for (addrinfo* p = res; p; p = p->ai_next) {
        char buf[INET_ADDRSTRLEN]{};
        auto* a = reinterpret_cast<sockaddr_in*>(p->ai_addr);
        if (!a) continue;
        if (inet_ntop(AF_INET, &a->sin_addr, buf, sizeof buf)) {
            std::string ip = buf;
            if (ip.rfind("127.", 0) != 0)
                out.push_back(ip);
        }
    }
    freeaddrinfo(res);
    return out;
}

bool batDauTuConsole(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, FileRepository& repo, int cong) {
    if (S.dangChay.load()) {
        std::cout << "Web dang chay san tai http://localhost:" << cong << "\n";
        return true;
    }
    S.ganTuConsole(ql, dv, nd, repo);
    if (S.ql) capNhatGioChuyenTheoThoiGianThuc(*S.ql);
    S.phien.clear();
    S.giu.clear();
    S.sanBay.clear();
    S.mayBay.clear();
    S.maxVe = 0;
    S.demGiu = 0;
    for (ChuyenBay* cb : S.ql->timChuyenBay("", "", time_point{})) biet(cb);
    for (long i = 1; i <= 5000; ++i) if (VeMayBay* v = S.dv->timVe(maVeTu(i))) ghiNhanVe(v);

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    sock_t ls = ::socket(AF_INET, SOCK_STREAM, 0);
    if (ls == INVALID_SOCKET) { std::cerr << "Khong tao duoc socket\n"; return false; }
    int bat = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&bat), sizeof bat);
    sockaddr_in dc{};
    dc.sin_family = AF_INET;
    dc.sin_port = htons(static_cast<unsigned short>(cong));
    inet_pton(AF_INET, "127.0.0.1", &dc.sin_addr);
    if (::bind(ls, reinterpret_cast<sockaddr*>(&dc), sizeof dc) != 0 || ::listen(ls, 64) != 0) {
        std::cerr << "Khong mo duoc cong " << cong << " (thu cong khac).\n";
        DONG_SOCK(ls);
        return false;
    }
    S.listenSock = ls;
    S.dangChay = true;
    std::thread([]() { vongLapChapNhan(); }).detach();
    std::cout << "\n  *** WEB da mo (dong bo live voi console) ***\n"
              << "  Mở trình duyệt:  http://localhost:" << cong << "\n"
              << "  Moi thao tac dat ve / them chuyen tren console se hien ngay tren web.\n"
              << "  Tai khoan: AD01/admin123  DL01/daily123  HK01/hk123  HK02/hk456\n\n";
    return true;
}
}  // namespace web
#endif  // BAT_WEB_DEMO


// ======================================================================
// HAM MAIN
// ======================================================================


// ===== main (cùng translation unit với helpers anonymous) =====
int main(int argc, char** argv) {
    batUtf8Console();
#if BAT_WEB_DEMO
    for (int i = 1; i < argc; ++i)
        if (std::string(argv[i]) == "--web") return web::chayWeb(argc, argv);
#else
    (void)argc; (void)argv;
#endif

    QuanLyChuyenBay qlChuyenBay;
    QuanLyDatVe qlDatVe(qlChuyenBay);
    QuanLyNguoiDung qlNguoiDung;
    FileRepository repo("data");
    try {
        if (std::filesystem::exists("data/nguoidung.csv")) {
            repo.docNguoiDung(qlNguoiDung);      // thu tu bat buoc: nguoi dung -> chuyen bay -> ve
            repo.docChuyenBay(qlChuyenBay);
            repo.docVe(qlDatVe);
            std::cout << "Đã nạp dữ liệu từ thư mục data/\n";
            xuatMaTranNoiChuyen(qlChuyenBay);
            capNhatGioChuyenTheoThoiGianThuc(qlChuyenBay);
        } else {
            nhapDuLieuMau(qlChuyenBay, qlNguoiDung);
            std::cout << "Lần chạy đầu: đã tạo dữ liệu mẫu.\n"
                      << "  Tai khoan: AD01/admin123 (Admin), DL01/daily123 (Dai ly), Hanh khach tu dang ky; 5 dai ly DL01-DL05\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Loi nap du lieu: " << e.what() << "\n";
        return 1;
    }
    Menu menu(qlChuyenBay, qlDatVe, qlNguoiDung, repo);
    menu.chay();
    return 0;
}
