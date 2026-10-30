// =====================================================================================
//  BTL OOP - DT04: HE THONG QUAN LY DAT VE & DIEU HANH CHUYEN BAY HANG KHONG
//  TOAN BO CHUONG TRINH TRONG 1 FILE (34 lop dung theo so do UML DT04_UML_FINAL.puml)
//
//  CACH BIEN DICH (C++17):
//    Linux / macOS :  g++ -std=c++17 -O1 BTL_OOP.cpp -o datve
//    Windows MinGW :  g++ -std=c++17 -O1 BTL_OOP.cpp -o datve.exe -lws2_32
//    Visual Studio :  tao Console project, them file nay, chon C++17 va them /utf-8
//    (Neu IDE bao loi winsock / link: doi BAT_WEB_DEMO ben duoi tu 1 thanh 0 -> chi con ban console)
//
//  CACH CHAY:
//    ./datve          -> ban CONSOLE (menu chu)
//    ./datve --web    -> mo WEB DEMO, vao trinh duyet:  http://localhost:8080
//                        (them --reset de xoa du lieu cu, --port 9000 de doi cong)
//
//  TAI KHOAN MAU:  AD01/admin123 (Admin)   DL01/daily123 (Dai ly)   HK01/hk123, HK02/hk456 (Hanh khach)
//  Du lieu luu trong thu muc  data/  (file CSV), tu nap lai khi chay lan sau.
//
//  BO CUC FILE:
//    PHAN 1  Enum                          PHAN 4  Du lieu mau + ham main
//    PHAN 2  Khai bao 34 lop (truoc la .h)    PHAN 5  Web demo (may chu HTTP + trang web nhung san)
//    PHAN 3  Cai dat cac lop (truoc la .cpp)
// =====================================================================================

#define BAT_WEB_DEMO 1   // 1 = co the chay web (--web). 0 = chi console, khong can winsock/socket.

#if BAT_WEB_DEMO && defined(_WIN32)
  #define NOMINMAX
  #define WIN32_LEAN_AND_MEAN
  #include <winsock2.h>
  #include <ws2tcpip.h>
#elif BAT_WEB_DEMO
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <unistd.h>
#endif

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

namespace fs = std::filesystem;
using time_point = std::chrono::system_clock::time_point;   // UML dung ten "time_point"

// Khai bao truoc tat ca cac lop (de con tro / tham chieu qua lai giua cac lop)
class NguoiDung;
class Admin;
class DaiLy;
class HanhKhach;
class SanBay;
class MayBay;
class HangHangKhong;
class Ghe;
class SoDoGhe;
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
}
#endif


// ======================================================================
// PHAN 1: ENUM
// ======================================================================
enum class HangGhe {
    THUONG_GIA,
    PHO_THONG,
    TIET_KIEM,
};

enum class TrangThaiGhe {
    TRONG,
    GIU_CHO,
    DA_DAT,
};

// ======================================================================
// PHAN 2: KHAI BAO CAC LOP (noi dung truoc day nam trong cac file .h)
// ======================================================================

// ----- NguoiDung -----
class NguoiDung {
    // [NGOAI UML] FileRepository can doc/ghi du lieu noi bo de luu file CSV
    friend class FileRepository;
public:
    NguoiDung(std::string ma, std::string ten, std::string mk);
    virtual ~NguoiDung();
    std::string getMaND() const;
    std::string getHoTen() const;
    bool dangNhap(std::string mk) const;
    virtual void hienMenu() = 0;
    virtual std::string vaiTro() const = 0;
protected:
    std::string maND;
    std::string hoTen;
    std::string matKhau;
};


// ----- Admin -----
class Admin : public NguoiDung {
public:
    Admin(std::string ma, std::string ten, std::string mk);
    void hienMenu() override;
    std::string vaiTro() const override;
};


// ----- DaiLy -----
class DaiLy : public NguoiDung {
public:
    DaiLy(std::string ma, std::string ten, std::string mk, double tyLe);
    void congHoaHong(double giaTriVe);
    double getTongHoaHong() const;
    void hienMenu() override;
    std::string vaiTro() const override;
private:
    double tyLeHoaHong{};
    double tongHoaHong{};
};


// ----- HanhKhach -----
class HanhKhach : public NguoiDung {
public:
    HanhKhach(std::string ma, std::string ten, std::string mk, std::string cccd, std::string sdt, std::string email);
    std::string getCCCD() const;
    std::string getSoDienThoai() const;
    std::string getEmail() const;
    void hienMenu() override;
    std::string vaiTro() const override;
private:
    std::string cccd;
    std::string soDienThoai;
    std::string email;
};


// ----- SanBay -----
class SanBay {
public:
    SanBay(std::string ma, std::string ten, std::string thanhPho);
    std::string getMaSB() const;
    std::string getTen() const;
    std::string getThanhPho() const;
private:
    std::string maSB;
    std::string ten;
    std::string thanhPho;
};


// ----- MayBay -----
class MayBay {
public:
    MayBay(std::string soHieu, std::string model, int gheTG, int ghePT, int gheTK);
    std::string getSoHieu() const;
    int getSoGhe(HangGhe h) const;
    int tongSoGhe() const;
private:
    std::string soHieu;
    std::string model;
    int soGheThuongGia{};
    int soGhePhoThong{};
    int soGheTietKiem{};
};


// ----- HangHangKhong -----
class HangHangKhong {
public:
    HangHangKhong(std::string ma, std::string ten);
    std::string getMaHang() const;
    std::string getTen() const;
    void themMayBay(MayBay* mb);
    void themChuyenBay(ChuyenBay* cb);
    int soChuyenBay() const;
private:
    std::string maHang;
    std::string ten;
    std::vector<MayBay*> dsMayBay;
    std::vector<ChuyenBay*> dsChuyenBay;
};


// ----- Ghe -----
class Ghe {
public:
    Ghe(std::string so, HangGhe h);
    std::string getSoGhe() const;
    HangGhe getHang() const;
    TrangThaiGhe getTrangThai() const;
    void setTrangThai(TrangThaiGhe tt);
private:
    std::string soGhe;
    HangGhe hang{};
    TrangThaiGhe trangThai{};
};


// ----- SoDoGhe -----
class SoDoGhe {
public:
    SoDoGhe(const MayBay& mb);
    Ghe* timGhe(std::string soGhe);
    std::vector<Ghe*> layGheTrong(HangGhe h);
    int demGheTrong(HangGhe h) const;
    bool giuGhe(std::string soGhe);
    void huyGiu(std::string soGhe);
    bool datGhe(std::string soGhe);
    void traGhe(std::string soGhe);
    void hienThiSoDo() const;
private:
    std::vector<Ghe> dsGhe;
};


// ----- GiuCho -----
class GiuCho {
public:
    GiuCho(ChuyenBay* cb, std::string ghe, HanhKhach* hk, std::chrono::seconds timeout);
    ChuyenBay* getChuyenBay() const;
    std::string getSoGhe() const;
    HanhKhach* getHanhKhach() const;
    bool conHieuLuc() const;
private:
    ChuyenBay* chuyenBay{};
    std::string soGhe;
    HanhKhach* hanhKhach{};
    std::chrono::steady_clock::time_point hetHan{};
};


// ----- ChuyenBay -----
class ChuyenBay {
public:
    ChuyenBay(std::string ma, SanBay* di, SanBay* den, MayBay* mb, time_point gioDi, time_point gioDen, double giaCoBan);
    std::string getMaCB() const;
    SanBay* getSanBayDi() const;
    SanBay* getSanBayDen() const;
    MayBay* getMayBay() const;
    time_point getGioDi() const;
    time_point getGioDen() const;
    double getGiaCoBan() const;
    SoDoGhe& getSoDoGhe();
    std::chrono::minutes thoiGianBay() const;
    GiuCho* giuGhe(std::string soGhe, HanhKhach* hk, std::chrono::seconds timeout);
    void huyGiu(std::string soGhe);
    void donDepGiuCho();
    void datGhe(std::string soGhe);
    void traGhe(std::string soGhe);
    bool operator<(const ChuyenBay& o) const;
    static bool soSanhTheoGia(const ChuyenBay& a, const ChuyenBay& b);
private:
    std::string maCB;
    SanBay* sanBayDi{};
    SanBay* sanBayDen{};
    MayBay* mayBay{};
    SoDoGhe soDoGhe;
    std::vector<std::unique_ptr<GiuCho> > dsGiuCho;
    std::chrono::system_clock::time_point gioDi{};
    std::chrono::system_clock::time_point gioDen{};
    double giaCoBan{};
};


// ----- HanhTrinh -----
class HanhTrinh {
public:
    void themChang(ChuyenBay* cb);
    void setHanhKhach(HanhKhach* hk);
    void setHang(HangGhe h);
    void setHanhLy(double kg);
    void setSuatAn(std::string mon, double gia);
    HanhKhach* getHanhKhach() const;
    HangGhe getHang() const;
    const std::vector<ChuyenBay*>& getDsChang() const;
    double getSoKgHanhLy() const;
    std::string getMonAn() const;
    double getGiaSuatAn() const;
    int soChang() const;
    std::chrono::minutes tongThoiGian() const;
private:
    HanhKhach* hanhKhach{};
    HangGhe hang{};
    std::vector<ChuyenBay*> dsChang;
    double soKgHanhLy{};
    std::string monAn;
    double giaSuatAn{};
};


// ----- VeMayBay -----
class VeMayBay {
public:
    VeMayBay(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk);
    virtual ~VeMayBay();
    std::string getMaVe() const;
    std::string getSoGhe() const;
    void setSoGhe(std::string ghe);
    ChuyenBay* getChuyenBay() const;
    HanhKhach* getHanhKhach() const;
    bool isDaCheckIn() const;
    void danhDauCheckIn();
    virtual double tinhGia() const = 0;
    virtual std::string loaiVe() const = 0;
    virtual std::string moTa() const;
    virtual const VeMayBay* veGoc() const;
    void inBoardingPass() const;
protected:
    std::string maVe;
    std::string soGhe;
    ChuyenBay* chuyenBay{};
    HanhKhach* hanhKhach{};
    std::chrono::system_clock::time_point ngayDat{};
    bool daCheckIn{};
};


// ----- VeThuongGia -----
class VeThuongGia : public VeMayBay {
public:
    VeThuongGia(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk);
    double tinhGia() const override;
    std::string loaiVe() const override;
    bool coPhongCho() const;
private:
    static constexpr double HE_SO = 2.5;
};


// ----- VePhoThong -----
class VePhoThong : public VeMayBay {
public:
    VePhoThong(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk);
    double tinhGia() const override;
    std::string loaiVe() const override;
private:
    static constexpr double HE_SO = 1.0;
};


// ----- VeTietKiem -----
class VeTietKiem : public VeMayBay {
public:
    VeTietKiem(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk);
    double tinhGia() const override;
    std::string loaiVe() const override;
    bool choPhepDoiVe() const;
private:
    static constexpr double HE_SO = 0.8;
};


// ----- VeDecorator -----
class VeDecorator : public VeMayBay {
public:
    VeDecorator(std::unique_ptr<VeMayBay> v);
    const VeMayBay* veGoc() const override;
    std::string loaiVe() const override;
    double tinhGia() const override = 0;
protected:
    std::unique_ptr<VeMayBay> ve;
};


// ----- DichVuGiaTang -----
class DichVuGiaTang {
public:
    DichVuGiaTang(std::string ma, std::string ten);
    virtual ~DichVuGiaTang();
    virtual double tinhPhi() const = 0;
    virtual std::string moTa() const = 0;
protected:
    std::string maDV;
    std::string ten;
};


// ----- HanhLy -----
class HanhLy : public DichVuGiaTang {
public:
    HanhLy(double kg, double donGiaKg);
    double tinhPhi() const override;
    std::string moTa() const override;
private:
    double soKg{};
    double donGiaKg{};
};


// ----- VeCoHanhLy -----
class VeCoHanhLy : public VeDecorator {
public:
    VeCoHanhLy(std::unique_ptr<VeMayBay> v, HanhLy hl);
    double tinhGia() const override;
    std::string moTa() const override;
private:
    HanhLy hanhLy;
};


// ----- SuatAn -----
class SuatAn : public DichVuGiaTang {
public:
    SuatAn(std::string mon, double gia);
    double tinhPhi() const override;
    std::string moTa() const override;
private:
    std::string monAn;
    double gia{};
};


// ----- VeCoSuatAn -----
class VeCoSuatAn : public VeDecorator {
public:
    VeCoSuatAn(std::unique_ptr<VeMayBay> v, SuatAn sa);
    double tinhGia() const override;
    std::string moTa() const override;
private:
    SuatAn suatAn;
};


// ----- VeFactory -----
class VeFactory {
public:
    static std::unique_ptr<VeMayBay> taoVe(HangGhe h, std::string maVe, std::string soGhe, ChuyenBay* cb, HanhKhach* hk);
};


// ----- HanhTrinhBuilder -----
class HanhTrinhBuilder {
public:
    HanhTrinhBuilder();
    HanhTrinhBuilder& themChang(ChuyenBay* cb);
    HanhTrinhBuilder& chonHang(HangGhe h);
    HanhTrinhBuilder& chonHanhKhach(HanhKhach* hk);
    HanhTrinhBuilder& themHanhLy(double kg);
    HanhTrinhBuilder& themSuatAn(std::string mon, double gia);
    HanhTrinh build();
private:
    HanhTrinh hanhTrinh;
};


// ----- QuanLyChuyenBay -----
class QuanLyChuyenBay {
    // [NGOAI UML] FileRepository can doc/ghi du lieu noi bo de luu file CSV
    friend class FileRepository;
public:
    void themChuyenBay(std::unique_ptr<ChuyenBay> cb);
    void xoaChuyenBay(std::string maCB);
    void themSanBay(std::unique_ptr<SanBay> sb);
    void themMayBay(std::unique_ptr<MayBay> mb);
    void themHang(std::unique_ptr<HangHangKhong> h);
    ChuyenBay* timChuyenBay(std::string maCB) const;
    SanBay* timSanBay(std::string maSB) const;
    MayBay* timMayBay(std::string soHieu) const;
    std::vector<ChuyenBay*> timChuyenBay(std::string di, std::string den, time_point ngay) const;
    std::vector<std::vector<ChuyenBay*> > timNoiChuyen(std::string di, std::string den, time_point ngay, std::chrono::minutes choToiThieu, int toiDaChang) const;
    void sapXepTheoGio(std::vector<ChuyenBay*>& ds);
    void sapXepTheoGia(std::vector<ChuyenBay*>& ds);
    // [BO SUNG UX] liet ke de menu/web hien danh sach chon (khong thay doi nghiep vu UML)
    std::vector<SanBay*> layDsSanBay() const;
    std::vector<MayBay*> layDsMayBay() const;
    std::vector<ChuyenBay*> layDsChuyenBay() const;
    std::vector<HangHangKhong*> layDsHang() const;
private:
    std::vector<std::unique_ptr<ChuyenBay> > dsChuyenBay;
    std::vector<std::unique_ptr<SanBay> > dsSanBay;
    std::vector<std::unique_ptr<MayBay> > dsMayBay;
    std::vector<std::unique_ptr<HangHangKhong> > dsHang;
};


// ----- QuanLyDatVe -----
class QuanLyDatVe {
    // [NGOAI UML] FileRepository can doc/ghi du lieu noi bo de luu file CSV
    friend class FileRepository;
public:
    QuanLyDatVe(QuanLyChuyenBay& ql);
    GiuCho* giuCho(std::string maCB, std::string soGhe, HanhKhach* hk);
    VeMayBay* xacNhanThanhToan(GiuCho* gc, HangGhe h, DaiLy* dl);
    std::vector<VeMayBay*> datVeTheoHanhTrinh(const HanhTrinh& ht, DaiLy* dl);
    void huyVe(std::string maVe);
    void checkIn(std::string maVe, std::string soGheMoi);
    void inBoardingPass(std::string maVe) const;
    VeMayBay* timVe(std::string maVe) const;
    std::map<std::string, int> thongKeLoaiVe() const;
    double tinhDoanhThu() const;
    // [BO SUNG UX] liet ke ve
    std::vector<VeMayBay*> layDsVe() const;
private:
    std::vector<std::unique_ptr<VeMayBay> > dsVe;
    QuanLyChuyenBay& qlChuyenBay;
};


// ----- QuanLyNguoiDung -----
class QuanLyNguoiDung {
    // [NGOAI UML] FileRepository can doc/ghi du lieu noi bo de luu file CSV
    friend class FileRepository;
public:
    void themNguoiDung(std::unique_ptr<NguoiDung> nd);
    NguoiDung* dangNhap(std::string ma, std::string mk) const;
    NguoiDung* timNguoiDung(std::string ma) const;
    // [BO SUNG UX] liet ke nguoi dung cho Admin/Dai ly
    std::vector<NguoiDung*> layDsNguoiDung() const;
private:
    std::vector<std::unique_ptr<NguoiDung> > dsNguoiDung;
};


// ----- FileRepository -----
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


// ----- Menu -----
class Menu {
public:
    Menu(QuanLyChuyenBay& qlCB, QuanLyDatVe& qlDV, QuanLyNguoiDung& qlND, FileRepository& repo);
    void chay();
private:
    QuanLyChuyenBay& qlChuyenBay;
    QuanLyDatVe& qlDatVe;
    QuanLyNguoiDung& qlNguoiDung;
    FileRepository& repo;
    void menuAdmin();
    void menuDaiLy();
    void menuHanhKhach();
};


// ----- LoiNghiepVu -----
class LoiNghiepVu : public std::runtime_error {
public:
    LoiNghiepVu(std::string msg);
};


// ----- LoiHetCho -----
class LoiHetCho : public LoiNghiepVu {
public:
    LoiHetCho(std::string msg);
};


// ----- LoiGiuChoHetHan -----
class LoiGiuChoHetHan : public LoiNghiepVu {
public:
    LoiGiuChoHetHan(std::string msg);
};


// ----- LoiChuyenBayKhongTonTai -----
class LoiChuyenBayKhongTonTai : public LoiNghiepVu {
public:
    LoiChuyenBayKhongTonTai(std::string msg);
};


// ----- LoiDinhDangFile -----
class LoiDinhDangFile : public LoiNghiepVu {
public:
    LoiDinhDangFile(std::string msg);
};


// ======================================================================
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
              << " 1. Xem danh sach chuyen bay\n"
              << " 2. Them san bay\n"
              << " 3. Them may bay\n"
              << " 4. Them chuyen bay\n"
              << " 5. Xoa chuyen bay\n"
              << " 6. Xem doanh thu\n"
              << " 7. Thong ke ve theo hang\n"
              << " 8. Luu du lieu\n"
              << " 9. Xem danh sach san bay / may bay\n"
              << "10. Xem tat ca ve\n"
              << "11. Xem danh sach nguoi dung\n"
              << "12. Chi tiet san bay (hang + may bay hoat dong)\n"
              << "13. Mo giao dien WEB (dong bo du lieu console)\n"
              << "14. Tinh trang ghe/ve tung chuyen (+ xuat file)\n"
              << "15. Quy dinh uu dai hang ve & hanh ly\n"
              << " 0. Dang xuat\n";
}

std::string Admin::vaiTro() const { return "Admin"; }


// ----- DaiLy.cpp -----
DaiLy::DaiLy(std::string ma, std::string ten, std::string mk, double tyLe)
    : NguoiDung(std::move(ma), std::move(ten), std::move(mk)), tyLeHoaHong(tyLe), tongHoaHong(0.0) {}

void DaiLy::congHoaHong(double giaTriVe) { tongHoaHong += giaTriVe * tyLeHoaHong; }
double DaiLy::getTongHoaHong() const { return tongHoaHong; }

void DaiLy::hienMenu() {
    std::cout << "\n===== MENU DAI LY =====\n"
              << " 1. Tim chuyen bay\n"
              << " 2. Xem so do ghe\n"
              << " 3. Dat ve ho khach\n"
              << " 4. Xem hoa hong\n"
              << " 5. Mo giao dien WEB (dong bo du lieu)\n"
              << " 6. Tinh trang ghe/ve tung chuyen (+ xuat file)\n"
              << " 7. Quy dinh uu dai hang ve & hanh ly\n"
              << " 0. Dang xuat\n";
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
    std::cout << "\n===== MENU HANH KHACH =====\n"
              << " 1. Tim chuyen bay\n"
              << " 2. Tim chuyen noi chuyen\n"
              << " 3. Xem so do ghe\n"
              << " 4. Dat ve (giu cho 5 phut)\n"
              << " 5. Dat ve theo hanh trinh (nhieu chang, hanh ly, suat an)\n"
              << " 6. Check-in chon cho\n"
              << " 7. In boarding pass\n"
              << " 8. Huy ve\n"
              << " 9. Mo giao dien WEB (dong bo du lieu)\n"
              << " 0. Dang xuat\n";
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
        std::cout << "  " << tenHang(h) << " (con " << demGheTrong(h) << " ghe trong)\n   ";
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
    if (!mb) throw LoiNghiepVu("Chuyen bay phai gan voi mot may bay");
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

// Giu cho tam thoi co timeout. Cac phien giu cho (GiuCho) duoc giu den khi chuyen bay bi huy
// de con tro GiuCho* tra ra ngoai khong bao gio bi treo (dangling).
GiuCho* ChuyenBay::giuGhe(std::string soGhe, HanhKhach* hk, std::chrono::seconds timeout) {
    donDepGiuCho();
    if (!soDoGhe.timGhe(soGhe))
        throw LoiHetCho("Ghe " + soGhe + " khong ton tai tren chuyen bay " + maCB);
    if (!soDoGhe.giuGhe(soGhe))
        throw LoiHetCho("Ghe " + soGhe + " da co nguoi dat hoac dang duoc giu");
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
        std::cout << " Chuyen bay : " << chuyenBay->getMaCB() << "\n"
                  << " Hanh trinh : " << (di ? di->getMaSB() : "?") << " -> " << (den ? den->getMaSB() : "?") << "\n"
                  << " Gio di     : " << dinhDangGio(chuyenBay->getGioDi()) << "\n"
                  << " Gio den    : " << dinhDangGio(chuyenBay->getGioDen()) << "\n";
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

std::string VeThuongGia::loaiVe() const { return "Thuong gia"; }

bool VeThuongGia::coPhongCho() const { return true; }


// ----- VePhoThong.cpp -----
VePhoThong::VePhoThong(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : VeMayBay(std::move(ma), std::move(ghe), cb, hk) {}

double VePhoThong::tinhGia() const { return chuyenBay->getGiaCoBan() * HE_SO; }

std::string VePhoThong::loaiVe() const { return "Pho thong"; }


// ----- VeTietKiem.cpp -----
VeTietKiem::VeTietKiem(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : VeMayBay(std::move(ma), std::move(ghe), cb, hk) {}

double VeTietKiem::tinhGia() const { return chuyenBay->getGiaCoBan() * HE_SO; }

std::string VeTietKiem::loaiVe() const { return "Tiet kiem"; }

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
        throw LoiNghiepVu("Chuyen bay " + maCB + " da co ve hoac dang giu cho, khong the xoa");
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
        throw LoiGiuChoHetHan("Giu cho ghe " + soGhe + " da het han, vui long dat lai");
    }
    Ghe* ghe = cb->getSoDoGhe().timGhe(soGhe);
    if (!ghe || ghe->getTrangThai() != TrangThaiGhe::GIU_CHO)
        throw LoiGiuChoHetHan("Phien giu cho ghe " + soGhe + " khong con hieu luc");
    if (ghe->getHang() != h) {
        cb->huyGiu(soGhe);
        throw LoiNghiepVu("Ghe " + soGhe + " khong thuoc hang ve da chon");
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
            if (trong.empty()) throw LoiHetCho("Chuyen bay " + cb->getMaCB() + " het ghe hang nay");
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
        if (!gheMoi) throw LoiNghiepVu("Ghe " + soGheMoi + " khong ton tai");
        if (gheCu && gheMoi->getHang() != gheCu->getHang())
            throw LoiNghiepVu("Chi duoc doi sang ghe cung hang ve");
        if (gheMoi->getTrangThai() != TrangThaiGhe::TRONG || !sd.datGhe(soGheMoi))
            throw LoiHetCho("Ghe " + soGheMoi + " da co nguoi dat hoac dang duoc giu");
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
    std::map<std::string, int> kq{{"Thuong gia", 0}, {"Pho thong", 0}, {"Tiet kiem", 0}};
    for (const auto& v : dsVe) {
        const VeMayBay* goc = v->veGoc();
        if (dynamic_cast<const VeThuongGia*>(goc)) ++kq["Thuong gia"];
        else if (dynamic_cast<const VePhoThong*>(goc)) ++kq["Pho thong"];
        else if (dynamic_cast<const VeTietKiem*>(goc)) ++kq["Tiet kiem"];
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
        if (f[1] == "Thuong gia") hang = HangGhe::THUONG_GIA;
        else if (f[1] == "Pho thong") hang = HangGhe::PHO_THONG;
        else if (f[1] == "Tiet kiem") hang = HangGhe::TIET_KIEM;
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
                f << ',' << dl->getTongHoaHong() << ",,";
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
            auto dl = std::make_unique<DaiLy>(f[1], f[2], f[3], TY_LE_HOA_HONG_MAC_DINH);
            const double tong = std::stod(f[4].empty() ? "0" : f[4]);
            if (tong > 0) dl->congHoaHong(tong / TY_LE_HOA_HONG_MAC_DINH);
            ql.dsNguoiDung.push_back(std::move(dl));
        } else if (f[0] == "Hanh khach") {
            ql.dsNguoiDung.push_back(std::make_unique<HanhKhach>(f[1], f[2], f[3], f[4], f[5], f[6]));
        } else {
            throw LoiDinhDangFile("vai tro la: " + f[0]);
        }
    });
    nguoiDungDaDoc = &ql;
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
        try { return std::stoi(s); } catch (...) { if (std::cin) std::cout << "Vui long nhap so nguyen.\n"; }
    }
    return 0;
}

double docSoThuc(const std::string& hoi) {
    while (std::cin) {
        const std::string s = docDong(hoi);
        try { return std::stod(s); } catch (...) { if (std::cin) std::cout << "Vui long nhap so.\n"; }
    }
    return 0;
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
    std::cout << " Hang ve:\n"
              << "  1. Thuong gia (x2.5)\n"
              << "  2. Pho thong (x1.0)\n"
              << "  3. Tiet kiem (x0.8)\n";
    while (std::cin) {
        const int c = docSo("Chon hang ve (1-3): ");
        if (c == 1) return HangGhe::THUONG_GIA;
        if (c == 2) return HangGhe::PHO_THONG;
        if (c == 3) return HangGhe::TIET_KIEM;
        std::cout << "  Lua chon khong hop le, nhap lai.\n";
    }
    return HangGhe::PHO_THONG;
}

void inDanhSachSanBay(const std::vector<SanBay*>& ds) {
    if (ds.empty()) { std::cout << "  (chua co san bay nao)\n"; return; }
    std::cout << std::left << "  " << std::setw(4) << "STT" << std::setw(8) << "Ma"
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
    if (ds.empty()) { std::cout << "  (khong co chuyen bay phu hop)\n"; return; }
    std::cout << std::left << "  " << std::setw(4) << "STT" << std::setw(8) << "Ma"
              << std::setw(13) << "Tuyen" << std::setw(13) << "Gio di"
              << std::setw(13) << "Gio den" << std::setw(18) << "Gia co ban" << "Ghe trong TG/PT/TK\n";
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
    if (choPhepTatCa) std::cout << "  0. Tat ca (khong loc)\n";
    while (std::cin) {
        const int stt = docSo(choPhepTatCa ? "Chon STT san bay (0 = tat ca): " : "Chon STT san bay: ");
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
        const int stt = docSo("Chon STT may bay: ");
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
        const int stt = docSo("Chon STT chuyen bay (0 = huy): ");
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
    std::cout << "  Ghe trong: ";
    for (std::size_t i = 0; i < trong.size(); ++i) {
        if (i) std::cout << ", ";
        std::cout << (i + 1) << "." << trong[i]->getSoGhe();
    }
    std::cout << "\n";
    while (std::cin) {
        const int stt = docSo("Chon STT ghe (hoac 0 de nhap tay so ghe): ");
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
        const int stt = docSo("Chon STT hanh khach: ");
        if (stt >= 1 && stt <= static_cast<int>(ds.size())) return ds[static_cast<std::size_t>(stt - 1)];
        std::cout << "  STT khong hop le.\n";
    }
    return nullptr;
}

void inVe(const VeMayBay* ve) {
    std::cout << "  " << ve->moTa() << " | Chuyen " << ve->getChuyenBay()->getMaCB() << " | Ghe " << ve->getSoGhe()
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
        const int stt = docSo("Chon STT ve (0 = nhap ma ve): ");
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
    const int stt = docSo("Chon STT san bay de xem chi tiet: ");
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
    std::cout << "\n-- Chuyen bay di/den tai day (" << lienQuan.size() << ") --\n";
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

void luuTatCa(FileRepository& repo, QuanLyChuyenBay& cb, QuanLyDatVe& dv, QuanLyNguoiDung& nd) {
    repo.ghiNguoiDung(nd);
    repo.ghiChuyenBay(cb);
    repo.ghiVe(dv);
    std::cout << "Da luu du lieu.\n";
}

void moWebTuMenu(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, FileRepository& repo) {
#if BAT_WEB_DEMO
    const int cong = docSo("Cong web (0 = 8080): ");
    const int c = cong > 0 ? cong : 8080;
    luuTatCa(repo, ql, dv, nd);
    if (!web::batDauTuConsole(ql, dv, nd, repo, c))
        std::cout << "Khong mo duoc web.\n";
#else
    (void)ql; (void)dv; (void)nd; (void)repo;
    std::cout << "Ban build khong bat BAT_WEB_DEMO - khong co web.\n";
#endif
}

// ---------- chuc nang dung chung ----------
void timChuyen(QuanLyChuyenBay& ql) {
    std::cout << "\n(Du lieu nap tu thu muc data/: sanbay.csv, chuyenbay.csv, maybay.csv, hang.csv)\n";
    std::cout << "Co " << ql.layDsSanBay().size() << " san bay, "
              << ql.layDsChuyenBay().size() << " chuyen bay, "
              << ql.layDsMayBay().size() << " may bay, "
              << ql.layDsHang().size() << " hang hang khong.\n";
    const std::string di = chonSanBayTuDs(ql, "San bay / thanh pho DI", true);
    const std::string den = chonSanBayTuDs(ql, "San bay / thanh pho DEN", true);
    const time_point ngay = docNgayLoc("Ngay bay dd/mm/yyyy (Enter = tat ca): ");
    std::vector<ChuyenBay*> ds = ql.timChuyenBay(di, den, ngay);
    std::cout << " Sap xep:\n  1. Theo gio di\n  2. Theo gia\n";
    if (docSo("Chon: ") == 2) ql.sapXepTheoGia(ds); else ql.sapXepTheoGio(ds);
    std::cout << "\n--- Ket qua tim chuyen ---\n";
    inDanhSach(ds);
}

void timNoiChuyen(QuanLyChuyenBay& ql) {
    const std::string di = chonSanBayTuDs(ql, "San bay DI (bat buoc)", false);
    const std::string den = chonSanBayTuDs(ql, "San bay DEN (bat buoc)", false);
    const time_point ngay = docNgayLoc("Ngay bay dd/mm/yyyy (Enter = tat ca): ");
    const auto kq = ql.timNoiChuyen(di, den, ngay, std::chrono::minutes(60), 3);
    if (kq.empty()) { std::cout << "  (khong co hanh trinh noi chuyen)\n"; return; }
    int stt = 0;
    for (const auto& duong : kq) {
        double tong = 0;
        std::cout << " Phuong an " << ++stt << ": ";
        for (std::size_t i = 0; i < duong.size(); ++i) {
            std::cout << duong[i]->getMaCB() << "(" << duong[i]->getSanBayDi()->getMaSB() << "->"
                      << duong[i]->getSanBayDen()->getMaSB() << " " << gioNgan(duong[i]->getGioDi()) << ")"
                      << (i + 1 < duong.size() ? " => " : "");
            tong += duong[i]->getGiaCoBan();
        }
        std::cout << " | tong gia co ban " << dinhDangTien(tong) << "\n";
    }
}

void xemSoDo(QuanLyChuyenBay& ql) {
    std::vector<ChuyenBay*> ds = ql.layDsChuyenBay();
    ql.sapXepTheoGio(ds);
    ChuyenBay* cb = chonChuyenBayTuDs(ds, "Chon chuyen bay de xem so do ghe");
    if (!cb) return;
    cb->donDepGiuCho();
    std::cout << "So do ghe chuyen " << cb->getMaCB() << ":\n";
    cb->getSoDoGhe().hienThiSoDo();
}

// Dat ve: giu cho 5 phut -> xac nhan thanh toan. dl != nullptr khi dai ly dat ho khach.

// Muc hanh ly mien phi (kg ky gui) theo hang ve
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
            return "x2.5 | HL mien phi 40kg+10kg xach tay | suat an kem | doi cho OK | phong cho TG";
        case HangGhe::PHO_THONG:
            return "x1.0 | HL mien phi 20kg+7kg xach tay | suat an them phi | doi cho cung hang OK";
        case HangGhe::TIET_KIEM:
            return "x0.8 | HL mien phi 15kg+7kg xach tay | suat an them phi | KHONG doi ve/cho";
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
    std::cout << "  Phi vuot muc: 50.000 VND/kg (ap dung phan kg vuot muc mien phi).\n"
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
            << " Chuyen " << cb->getMaCB() << " | "
            << cb->getSanBayDi()->getMaSB() << "->" << cb->getSanBayDen()->getMaSB()
            << " | may bay " << mb->getSoHieu() << "\n"
            << " Gio di " << gioNgan(cb->getGioDi()) << " | Gio den " << gioNgan(cb->getGioDen())
            << " | Gia co ban " << dinhDangTien(cb->getGiaCoBan()) << "\n"
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
    ChuyenBay* cb = chonChuyenBayTuDs(ds, "Chon chuyen bay de dat ve");
    if (!cb) { std::cout << "Huy dat ve.\n"; return; }
    const std::string maCB = cb->getMaCB();
    const HangGhe h = chonHang();
    std::cout << "  Uu dai: " << moTaUuDaiHang(h) << "\n";
    std::cout << "  Con " << cb->getSoDoGhe().demGheTrong(h) << " ghe hang nay; HL mien phi "
              << hanhLyMienPhiKg(h) << " kg.\n";
    const std::string soGhe = chonGheTuSoDo(cb, &h);
    if (soGhe.empty()) { std::cout << "Huy dat ve (khong chon ghe).\n"; return; }
    GiuCho* gc = dv.giuCho(maCB, soGhe, hk);
    std::cout << "Da giu cho ghe " << soGhe << " trong 5 phut.\n";
    const std::string xn = docDong("Xac nhan thanh toan? (y/n): ");
    if (xn == "y" || xn == "Y") {
        VeMayBay* ve = dv.xacNhanThanhToan(gc, h, dl);
        std::cout << "Dat ve thanh cong!\n";
        inVe(ve);
    } else {
        cb->huyGiu(soGhe);
        std::cout << "Da huy giu cho.\n";
    }
}

// Dat ve theo hanh trinh: dung HanhTrinhBuilder (nhieu chang + hanh ly + suat an).
void datVeHanhTrinh(QuanLyChuyenBay& cbl, QuanLyDatVe& dv, HanhKhach* hk, DaiLy* dl) {
    std::vector<ChuyenBay*> all = cbl.layDsChuyenBay();
    cbl.sapXepTheoGio(all);
    HanhTrinhBuilder b;
    const HangGhe hangChon = chonHang();
    b.chonHanhKhach(hk).chonHang(hangChon);
    std::cout << "  Uu dai: " << moTaUuDaiHang(hangChon) << "\n";
    int dem = 0;
    while (std::cin) {
        std::cout << "\nThem chang " << (dem + 1) << " (0 = xong):\n";
        ChuyenBay* cb = chonChuyenBayTuDs(all, "Chon chuyen bay cho chang nay");
        if (!cb) break;
        // Canh bao neu khoang hang da het ghe
        cb->donDepGiuCho();
        const int con = cb->getSoDoGhe().demGheTrong(hangChon);
        if (con == 0) {
            std::cout << "  Canh bao: chuyen " << cb->getMaCB() << " HET ghe hang da chon.\n";
            continue;
        }
        std::cout << "  Con " << con << " ghe hang nay tren chuyen " << cb->getMaCB() << ".\n";
        b.themChang(cb);
        ++dem;
    }
    if (dem == 0) { std::cout << "Chua chon chang nao.\n"; return; }
    std::cout << "\n--- Hanh ly (mien phi " << hanhLyMienPhiKg(hangChon) << " kg ky gui) ---\n";
    const double kg = docSoThuc("So kg hanh ly VUOT MUC mien phi (0 = khong): ");
    canhBaoHanhLy(hangChon, kg);
    if (kg > 0) b.themHanhLy(kg);
    const std::string mon = lamSachNhap(docDong("Mon an (Enter = khong dat suat an): "));
    if (!mon.empty()) b.themSuatAn(mon, docSoThuc("Gia suat an (VND): "));
    const HanhTrinh ht = b.build();
    const std::vector<VeMayBay*> ds = dv.datVeTheoHanhTrinh(ht, dl);
    double tong = 0;
    for (const VeMayBay* ve : ds) { inVe(ve); tong += ve->tinhGia(); }
    std::cout << "Dat thanh cong " << ds.size() << " ve. Tong tien: " << dinhDangTien(tong) << "\n";
}


}  // namespace (Menu helpers)

Menu::Menu(QuanLyChuyenBay& qlCB, QuanLyDatVe& qlDV, QuanLyNguoiDung& qlND, FileRepository& repo)
    : qlChuyenBay(qlCB), qlDatVe(qlDV), qlNguoiDung(qlND), repo(repo) {}

void Menu::chay() {
    std::cout << "=== HE THONG QUAN LY DAT VE & DIEU HANH CHUYEN BAY ===\n";
    while (std::cin) {
        std::cout << "\n 1. Dang nhap\n 0. Thoat\n";
        const int c = docSo("Chon: ");
        if (c == 0 || !std::cin) break;
        if (c != 1) continue;
        const std::string ma = docDong("Ma nguoi dung: ");
        const std::string mk = docDong("Mat khau: ");
        NguoiDung* nd = qlNguoiDung.dangNhap(ma, mk);
        if (!nd) { std::cout << "Sai ma nguoi dung hoac mat khau.\n"; continue; }
        nguoiHienTai = nd;
        std::cout << "Xin chao " << nd->getHoTen() << " (" << nd->vaiTro() << ")\n";
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
        const int c = docSo("Chon: ");
        if (c == 0 || !std::cin) break;
        try {
            switch (c) {
                case 1: {
                    std::vector<ChuyenBay*> ds = qlChuyenBay.timChuyenBay("", "", time_point{});
                    qlChuyenBay.sapXepTheoGio(ds);
                    inDanhSach(ds);
                    break;
                }
                case 2: {
                    const std::string ma = lamSachNhap(docDong("Ma san bay: "));
                    const std::string ten = lamSachNhap(docDong("Ten san bay: "));
                    const std::string tp = lamSachNhap(docDong("Thanh pho: "));
                    if (ma.empty()) throw LoiNghiepVu("Ma san bay khong duoc de trong");
                    qlChuyenBay.themSanBay(std::make_unique<SanBay>(ma, ten, tp));
                    std::cout << "Da them san bay " << ma << ".\n";
                    std::cout << "\n-- Cac hang hang khong hien co (co the khai thac tai san bay moi) --\n";
                    for (HangHangKhong* h : qlChuyenBay.layDsHang())
                        std::cout << "  " << h->getMaHang() << " - " << h->getTen()
                                  << " (" << h->soChuyenBay() << " chuyen)\n";
                    std::cout << "-- May bay hien co --\n";
                    inDanhSachMayBay(qlChuyenBay.layDsMayBay());
                    break;
                }
                case 3: {
                    const std::string soHieu = lamSachNhap(docDong("So hieu may bay: "));
                    const std::string model = lamSachNhap(docDong("Model: "));
                    const int tg = docSo("So ghe thuong gia: ");
                    const int pt = docSo("So ghe pho thong: ");
                    const int tk = docSo("So ghe tiet kiem: ");
                    if (soHieu.empty() || tg < 0 || pt < 0 || tk < 0 || tg + pt + tk == 0)
                        throw LoiNghiepVu("Thong tin may bay khong hop le");
                    qlChuyenBay.themMayBay(std::make_unique<MayBay>(soHieu, model, tg, pt, tk));
                    std::cout << "Da them may bay " << soHieu << ".\n";
                    std::cout << "\n-- Gan may bay vao hang (tuy chon, Enter bo qua) --\n";
                    {
                        auto dsH = qlChuyenBay.layDsHang();
                        for (std::size_t i = 0; i < dsH.size(); ++i)
                            std::cout << "  " << (i + 1) << ". " << dsH[i]->getMaHang() << " - " << dsH[i]->getTen() << "\n";
                        if (!dsH.empty()) {
                            const int sh = docSo("Chon STT hang (0 = bo qua): ");
                            if (sh >= 1 && sh <= static_cast<int>(dsH.size())) {
                                dsH[static_cast<std::size_t>(sh - 1)]->themMayBay(qlChuyenBay.timMayBay(soHieu));
                                std::cout << "Da gan may bay vao hang " << dsH[static_cast<std::size_t>(sh - 1)]->getMaHang() << ".\n";
                            }
                        }
                    }
                    break;
                }
                case 4: {
                    const std::string ma = lamSachNhap(docDong("Ma chuyen bay: "));
                    const std::string maDi = chonSanBayTuDs(qlChuyenBay, "San bay DI", false);
                    const std::string maDen = chonSanBayTuDs(qlChuyenBay, "San bay DEN", false);
                    SanBay* di = qlChuyenBay.timSanBay(maDi);
                    SanBay* den = qlChuyenBay.timSanBay(maDen);
                    MayBay* mb = chonMayBayTuDs(qlChuyenBay);
                    if (ma.empty() || !di || !den || !mb || di == den)
                        throw LoiNghiepVu("Ma chuyen bay / san bay / may bay khong hop le");
                    time_point gioDi, gioDen;
                    if (!docGio("Gio di (dd/mm/yyyy hh:mm): ", gioDi) || !docGio("Gio den (dd/mm/yyyy hh:mm): ", gioDen))
                        throw LoiNghiepVu("Dinh dang gio khong dung");
                    if (gioDen <= gioDi) throw LoiNghiepVu("Gio den phai sau gio di");
                    const double gia = docSoThuc("Gia co ban (VND): ");
                    if (gia <= 0) throw LoiNghiepVu("Gia phai lon hon 0");
                    qlChuyenBay.themChuyenBay(std::make_unique<ChuyenBay>(ma, di, den, mb, gioDi, gioDen, gia));
                    std::cout << "Da them chuyen bay " << ma << ".\n";
                    break;
                }
                case 5: {
                    std::vector<ChuyenBay*> ds = qlChuyenBay.layDsChuyenBay();
                    qlChuyenBay.sapXepTheoGio(ds);
                    ChuyenBay* cb = chonChuyenBayTuDs(ds, "Chon chuyen bay can xoa");
                    if (!cb) { std::cout << "Huy xoa.\n"; break; }
                    qlChuyenBay.xoaChuyenBay(cb->getMaCB());
                    std::cout << "Da xoa chuyen bay.\n";
                    break;
                }
                case 6:
                    std::cout << "Tong doanh thu ve: " << dinhDangTien(qlDatVe.tinhDoanhThu()) << "\n";
                    break;
                case 7:
                    for (const auto& kv : qlDatVe.thongKeLoaiVe()) std::cout << "  " << kv.first << ": " << kv.second << " ve\n";
                    break;
                case 8:
                    luuTatCa(repo, qlChuyenBay, qlDatVe, qlNguoiDung);
                    break;
                case 9: {
                    std::cout << "\n=== SAN BAY ===\n";
                    inDanhSachSanBay(qlChuyenBay.layDsSanBay());
                    std::cout << "\n=== MAY BAY ===\n";
                    inDanhSachMayBay(qlChuyenBay.layDsMayBay());
                    break;
                }
                case 10: {
                    std::cout << "\n=== TAT CA VE ===\n";
                    inDanhSachVe(qlDatVe.layDsVe());
                    break;
                }
                case 11: {
                    std::cout << "\n=== NGUOI DUNG ===\n";
                    for (NguoiDung* nd : qlNguoiDung.layDsNguoiDung()) {
                        std::cout << "  " << nd->getMaND() << " | " << nd->getHoTen()
                                  << " | " << nd->vaiTro();
                        if (auto* dlx = dynamic_cast<DaiLy*>(nd))
                            std::cout << " | hoa hong " << dinhDangTien(dlx->getTongHoaHong());
                        if (auto* hk = dynamic_cast<HanhKhach*>(nd))
                            std::cout << " | " << hk->getSoDienThoai() << " | " << hk->getEmail();
                        std::cout << "\n";
                    }
                    break;
                }
                
                case 12:
                    chiTietSanBay(qlChuyenBay);
                    break;
                case 13:
                    moWebTuMenu(qlChuyenBay, qlDatVe, qlNguoiDung, repo);
                    break;
                case 14:
                    baoCaoTinhTrangChuyenBay(qlChuyenBay, true);
                    break;
                case 15:
                    inQuyDinhUuDaiVaHanhLy();
                    break;
default:
                    std::cout << "Lua chon khong hop le.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Loi: " << e.what() << "\n";
        }
    }
}

void Menu::menuDaiLy() {
    DaiLy* dl = dynamic_cast<DaiLy*>(nguoiHienTai);
    while (std::cin) {
        nguoiHienTai->hienMenu();
        const int c = docSo("Chon: ");
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
                    moWebTuMenu(qlChuyenBay, qlDatVe, qlNguoiDung, repo);
                    break;
                case 6:
                    baoCaoTinhTrangChuyenBay(qlChuyenBay, true);
                    break;
                case 7:
                    inQuyDinhUuDaiVaHanhLy();
                    break;
                default:
                    std::cout << "Lua chon khong hop le.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Loi: " << e.what() << "\n";
        }
    }
}

void Menu::menuHanhKhach() {
    HanhKhach* hk = dynamic_cast<HanhKhach*>(nguoiHienTai);
    while (std::cin) {
        nguoiHienTai->hienMenu();
        const int c = docSo("Chon: ");
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
                    moWebTuMenu(qlChuyenBay, qlDatVe, qlNguoiDung, repo);
                    break;
                default:
                    std::cout << "Lua chon khong hop le.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Loi: " << e.what() << "\n";
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
    themChuyen("VN101", "SGN", "HAN", "VN-A001", 8*60, 130, 1500000);
    themChuyen("VN102", "SGN", "HAN", "VN-A002", 14*60, 130, 1350000);
    themChuyen("VN103", "HAN", "SGN", "VN-A001", 18*60, 130, 1480000);
    themChuyen("VN104", "HAN", "SGN", "VN-A003", 22*60, 130, 1600000);
    themChuyen("VN201", "SGN", "DAD", "VN-A001", 10*60, 80, 900000);
    themChuyen("VN202", "DAD", "HAN", "VN-A002", 14*60, 80, 800000);
    themChuyen("VN203", "HAN", "DAD", "VN-A002", 20*60, 80, 820000);
    themChuyen("VN301", "SGN", "PQC", "VN-A002", 12*60, 60, 700000);
    themChuyen("VN302", "PQC", "SGN", "VN-A001", 28*60, 60, 720000);
    themChuyen("VN401", "SGN", "CXR", "VN-A001", 16*60, 70, 850000);
    themChuyen("VN402", "CXR", "SGN", "VN-A002", 26*60, 70, 860000);
    themChuyen("VN501", "HAN", "HPH", "VN-A003", 9*60, 45, 500000);
    themChuyen("VN502", "SGN", "VCA", "VN-A001", 11*60, 55, 650000);
    themChuyen("VN503", "SGN", "DLI", "VN-A002", 15*60, 55, 750000);
    themChuyen("VN504", "HAN", "HUI", "VN-A003", 13*60, 70, 780000);
    // VietJet
    themChuyen("VJ101", "SGN", "HAN", "VJ-B001", 9*60, 130, 990000);
    themChuyen("VJ102", "HAN", "SGN", "VJ-B002", 17*60, 130, 980000);
    themChuyen("VJ201", "SGN", "DAD", "VJ-B001", 11*60, 80, 690000);
    themChuyen("VJ202", "SGN", "PQC", "VJ-B002", 13*60, 60, 550000);
    themChuyen("VJ301", "SGN", "CXR", "VJ-B001", 19*60, 70, 620000);
    themChuyen("VJ401", "HAN", "DAD", "VJ-B002", 21*60, 80, 710000);
    themChuyen("VJ501", "SGN", "VII", "VJ-B001", 24*60, 100, 800000);
    // Bamboo
    themChuyen("QH101", "SGN", "HAN", "QH-C001", 12*60, 130, 1200000);
    themChuyen("QH102", "HAN", "SGN", "QH-C002", 20*60, 130, 1180000);
    themChuyen("QH201", "SGN", "HUI", "QH-C001", 15*60, 75, 880000);
    themChuyen("QH301", "HAN", "VDO", "QH-C002", 10*60, 50, 450000);
    // Vietravel
    themChuyen("VU101", "SGN", "HAN", "VU-D001", 16*60, 130, 1100000);
    themChuyen("VU201", "SGN", "DAD", "VU-D001", 22*60, 80, 750000);
    themChuyen("VU301", "SGN", "UIH", "VU-D001", 25*60, 75, 720000);

    auto vna = std::make_unique<HangHangKhong>("VNA", "Vietnam Airlines");
    for (const char* m : {"VN-A001","VN-A002","VN-A003"}) vna->themMayBay(ql.timMayBay(m));
    for (const char* m : {"VN101","VN102","VN103","VN104","VN201","VN202","VN203","VN301","VN302","VN401","VN402","VN501","VN502","VN503","VN504"})
        vna->themChuyenBay(ql.timChuyenBay(m));
    ql.themHang(std::move(vna));

    auto vj = std::make_unique<HangHangKhong>("VJ", "VietJet Air");
    for (const char* m : {"VJ-B001","VJ-B002"}) vj->themMayBay(ql.timMayBay(m));
    for (const char* m : {"VJ101","VJ102","VJ201","VJ202","VJ301","VJ401","VJ501"}) vj->themChuyenBay(ql.timChuyenBay(m));
    ql.themHang(std::move(vj));

    auto qh = std::make_unique<HangHangKhong>("QH", "Bamboo Airways");
    for (const char* m : {"QH-C001","QH-C002"}) qh->themMayBay(ql.timMayBay(m));
    for (const char* m : {"QH101","QH102","QH201","QH301"}) qh->themChuyenBay(ql.timChuyenBay(m));
    ql.themHang(std::move(qh));

    auto vu = std::make_unique<HangHangKhong>("VU", "Vietravel Airlines");
    vu->themMayBay(ql.timMayBay("VU-D001"));
    for (const char* m : {"VU101","VU201","VU301"}) vu->themChuyenBay(ql.timChuyenBay(m));
    ql.themHang(std::move(vu));

    nd.themNguoiDung(std::make_unique<Admin>("AD01", "Quan tri vien", "admin123"));
    nd.themNguoiDung(std::make_unique<DaiLy>("DL01", "Dai ly Sao Viet", "daily123", 0.05));
    nd.themNguoiDung(std::make_unique<HanhKhach>("HK01", "Nguyen Van An", "hk123", "079200001111", "0901111111", "an@mail.com"));
    nd.themNguoiDung(std::make_unique<HanhKhach>("HK02", "Tran Thi Binh", "hk456", "079200002222", "0902222222", "binh@mail.com"));
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
<title>SkyBook � Demo BTL OOP �T04</title>
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
.hood{background:#0f172a;color:#cbd5e1;border-radius:10px;padding:10px 14px;margin-top:12px;font:12.5px/1.7 ui-monospace,Consolas,monospace}
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
.m-ok{background:var(--ok)}.m-err{background:var(--bad)}
.modal{position:fixed;inset:0;background:rgba(15,23,42,.55);display:flex;align-items:center;justify-content:center;z-index:50;padding:16px}
.modal>div{background:#fff;border-radius:var(--r);padding:20px;max-width:560px;width:100%;max-height:90vh;overflow:auto}
pre.bp{background:#0f172a;color:#e2e8f0;padding:14px;border-radius:10px;font:13px/1.5 ui-monospace,Consolas,monospace;overflow:auto}
footer{text-align:center;color:var(--mut);font-size:12px;padding:20px}
.right{text-align:right}.mut{color:var(--mut)}.mb0{margin-bottom:0}
</style>
</head>
<body>
<header>
  <div><h1>? SkyBook <small>Demo BTL OOP � �T04 � H? th?ng Qu?n l? �?t v� &amp; �i?u h�nh Chuy?n bay</small></h1></div>
  <div class="sp"></div>
  <div id="who"></div>
</header>
<main id="app"></main>
<footer>Backend: C++17 � g?i tr?c ti?p 34 l?p theo s� �? UML (Factory Method � Decorator � Builder) � Giao di?n ch? l� l?p v? g?i API</footer>
<div id="toast"></div>
<div id="modal"></div>

<script>
const $ = (s, r = document) => r.querySelector(s);
const $$ = (s, r = document) => [...r.querySelectorAll(s)];
const esc = s => String(s ?? '').replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const vnd = n => Number(n).toLocaleString('vi-VN') + ' �';
const hm = s => { const [d, t] = s.split(' '); const [y, m, dd] = d.split('-'); return `${t} � ${dd}/${m}`; };
const dur = p => `${Math.floor(p / 60)}g${String(p % 60).padStart(2, '0')}`;
const HANG = { TG: 'Th��ng gia', PT: 'Ph? th�ng', TK: 'Ti?t ki?m' };
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
  if (!d.ok) throw new Error(d.loi || 'L?i kh�ng x�c �?nh');
  return d;
}
const post = (p, a) => api(p, a, 'POST');
function toast(msg, ok = true) {
  const d = document.createElement('div'); d.className = ok ? 'm-ok' : 'm-err'; d.textContent = msg;
  $('#toast').appendChild(d); setTimeout(() => d.remove(), ok ? 3500 : 6000);
}
const guard = fn => async (...a) => { try { await fn(...a); } catch (e) { toast(e.message, false); } };
function modal(html) { $('#modal').innerHTML = `<div class="modal" onclick="if(event.target===this)closeModal()"><div>${html}</div></div>`; }
function closeModal() { $('#modal').innerHTML = ''; }
const hood = lines => `<div class="hood"><b>?? B�n trong (theo UML):</b><br>${lines.join('<br>')}</div>`;

/* ---------------- ��NG NH?P ---------------- */
function loginView() {
  $('#who').innerHTML = '';
  $('#app').innerHTML = `
  <div class="card"><h2>Ch?n t�i kho?n demo</h2>
    <div class="acc">
      <button data-u="HK01|hk123"><b>?? Nguy?n V�n An</b><span>H�nh kh�ch � HK01 / hk123</span></button>
      <button data-u="HK02|hk456"><b>?? Tr?n Th? B?nh</b><span>H�nh kh�ch � HK02 / hk456</span></button>
      <button data-u="DL01|daily123"><b>?? �?i l? Sao Vi?t</b><span>�?i l? (hoa h?ng 5%) � DL01 / daily123</span></button>
      <button data-u="AD01|admin123"><b>?? Qu?n tr? vi�n</b><span>Admin � AD01 / admin123</span></button>
    </div></div>
  <div class="card"><h2>Ho?c ��ng nh?p th? c�ng</h2>
    <form id="lf" class="grid"><div><label>M? ng�?i d�ng</label><input name="ma" autocomplete="off"></div>
    <div><label>M?t kh?u</label><input name="mk" type="password"></div><div><button class="b">��ng nh?p</button></div></form></div>`;
  $$('.acc button').forEach(b => b.onclick = guard(() => { const [ma, mk] = b.dataset.u.split('|'); return login(ma, mk); }));
  $('#lf').onsubmit = guard(e => { e.preventDefault(); const f = new FormData(e.target); return login(f.get('ma'), f.get('mk')); });
}
async function login(ma, mk) {
  const d = await post('login', { ma, mk }); token = d.token; user = d.user; sessionStorage.setItem('token', token);
  toast('Xin ch�o ' + user.ten); await boot();
}
async function logout() { try { await post('logout'); } catch (e) {} token = null; user = null; sessionStorage.removeItem('token'); clearInterval(S.timer); S.hold = null; loginView(); }

/* ---------------- KHUNG CH�NH ---------------- */
const VAITRO = { HK: 'H�nh kh�ch', DL: '�?i l?', AD: 'Qu?n tr? vi�n' };
const TABS = {
  HK: [['tim', '?? T?m chuy?n'], ['noi', '?? N?i chuy?n'], ['dat', '?? �?t v�'], ['ht', '?? H�nh tr?nh (Builder)'], ['ve', '?? V� c?a t�i']],
  DL: [['tim', '?? T?m chuy?n'], ['noi', '?? N?i chuy?n'], ['dat', '?? �?t v� h?'], ['ht', '?? H�nh tr?nh (Builder)'], ['ve', '?? V� �? b�n'], ['tk', '?? Hoa h?ng']],
  AD: [['qly', '?? Chuy?n bay'], ['ve', '?? T?t c? v�'], ['tk', '?? Th?ng k�']]
};
async function boot() {
  if (!user) { try { user = (await api('me')).user; } catch (e) { token = null; sessionStorage.removeItem('token'); return loginView(); } }
  meta = await api('meta');
  $('#who').innerHTML = `<span class="chip">${VAITRO[user.vaiTro]} � ${esc(user.ten)} (${esc(user.ma)})</span> <button class="b g s" id="lo">��ng xu?t</button>`;
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
  try { await ({ tim: pTim, noi: pNoi, dat: pDat, ht: pHT, ve: pVe, tk: pTK, qly: pQly })[t](); } catch (e) { toast(e.message, false); }
}
const apOpts = (all = true) => (all ? '<option value="">T?t c?</option>' : '') + meta.sanBay.map(s => `<option value="${s.ma}">${s.ma} � ${esc(s.tp)}</option>`).join('');

/* ---------------- T?M CHUY?N ---------------- */
function flightRows(list, action) {
  if (!list.length) return '<div class="empty">Kh�ng c� chuy?n bay ph� h?p</div>';
  return `<table><tr><th>M?</th><th>Tuy?n</th><th>Kh?i h�nh</th><th>�?n</th><th>Bay</th><th class="right">Gi� c� b?n</th><th>Gh? tr?ng</th><th></th></tr>` + list.map(c => `
    <tr><td><b>${c.ma}</b><br><small class="mut">${c.mayBay}</small></td><td>${c.di} ? ${c.den}<br><small class="mut">${esc(c.diTen)} ? ${esc(c.denTen)}</small></td>
    <td>${hm(c.gioDi)}</td><td>${hm(c.gioDen)}</td><td>${dur(c.phut)}</td><td class="right"><b>${vnd(c.gia)}</b></td>
    <td><span class="tag t-tg">TG ${c.TG.trong}</span> <span class="tag t-pt">PT ${c.PT.trong}</span> <span class="tag t-tk">TK ${c.TK.trong}</span></td>
    <td class="right">${action(c)}</td></tr>`).join('') + '</table>';
}
async function pTim() {
  $('#panel').innerHTML = `<div class="card"><h2>T?m chuy?n bay</h2><form id="f" class="grid">
    <div><label>T?</label><select name="di">${apOpts()}</select></div><div><label>�?n</label><select name="den">${apOpts()}</select></div>
    <div><label>Ng�y bay</label><input type="date" name="ngay"></div>
    <div><label>S?p x?p</label><select name="sapxep"><option value="gio">Theo gi? �i</option><option value="gia">Theo gi�</option></select></div>
    <div><button class="b">T?m</button></div></form>
    <div class="hood" style="margin-top:12px"><b>??</b> <em>QuanLyChuyenBay::timChuyenBay(di, den, ngay)</em> ? <em>sapXepTheoGio / sapXepTheoGia</em> (d�ng <em>ChuyenBay::operator&lt;</em> v� <em>soSanhTheoGia</em>)</div></div>
    <div class="card" id="res"></div>`;
  const run = guard(async () => {
    const f = Object.fromEntries(new FormData($('#f')));
    const d = await api('chuyenbay', f);
    $('#res').innerHTML = flightRows(d.chuyenBay, c => `<button class="b s" data-m="${c.ma}">Ch?n gh?</button>`);
    $$('#res [data-m]').forEach(b => b.onclick = () => { S.ma = b.dataset.m; go('dat'); });
  });
  $('#f').onsubmit = e => { e.preventDefault(); run(); }; run();
}

/* ---------------- N?I CHUY?N ---------------- */
async function pNoi() {
  $('#panel').innerHTML = `<div class="card"><h2>T?m h�nh tr?nh n?i chuy?n</h2><form id="f" class="grid">
    <div><label>T?</label><select name="di">${apOpts(false)}</select></div><div><label>�?n</label><select name="den">${apOpts(false)}</select></div>
    <div><label>Ng�y bay (tu? ch?n)</label><input type="date" name="ngay"></div>
    <div><label>Ch? t?i thi?u (ph�t)</label><input type="number" name="cho" value="60" min="0"></div>
    <div><button class="b">T?m n?i chuy?n</button></div></form>
    <div class="hood" style="margin-top:12px"><b>??</b> <em>QuanLyChuyenBay::timNoiChuyen(di, den, ngay, choToiThieu, toiDaChang)</em> � duy?t �? th? s�n bay, ch?ng sau ph?i kh?i h�nh sau khi ch?ng tr�?c �?n + th?i gian ch?</div></div>
    <div id="res"></div>`;
  const sel = $('#f [name=den]'); if (sel.options.length > 1) sel.selectedIndex = 1;
  $('#f').onsubmit = guard(async e => {
    e.preventDefault(); const f = Object.fromEntries(new FormData(e.target)); const d = await api('noichuyen', f);
    $('#res').innerHTML = !d.hanhTrinh.length ? '<div class="card empty">Kh�ng c� h�nh tr?nh n?i chuy?n ph� h?p (th? gi?m th?i gian ch? t?i thi?u)</div>' :
      d.hanhTrinh.map((h, i) => `<div class="card path"><b>Ph��ng �n ${i + 1}</b> � ${h.chang.length} ch?ng � t?ng ${dur(h.tongPhut)} � gi� c� b?n <b>${vnd(h.tongGia)}</b>` +
        h.chang.map(c => `<div class="leg"><b>${c.ma}</b> ${c.di} ? ${c.den} <span class="mut">${hm(c.gioDi)} ? ${hm(c.gioDen)}</span></div>`).join('') +
        `<button class="b s" data-i="${i}">�?t h�nh tr?nh n�y</button></div>`).join('');
    $$('#res [data-i]').forEach(b => b.onclick = () => { S.legs = d.hanhTrinh[b.dataset.i].chang.map(c => c.ma); go('ht'); });
  });
}

/* ---------------- S� �? GH? ---------------- */
function seatMap(ghe, o = {}) {
  const gr = { TG: [], PT: [], TK: [] }; ghe.forEach(g => gr[g.hang].push(g));
  let h = '<div class="seatmap">';
  for (const k of ['TG', 'PT', 'TK']) {
    if (!gr[k].length) continue; const per = k === 'TG' ? 4 : 6, rows = {};
    gr[k].forEach(g => (rows[parseInt(g.so)] = rows[parseInt(g.so)] || []).push(g));
    h += `<div class="cabin"><div class="cabin-title">${HANG[k]} <small>(${gr[k].filter(g => g.tt === 'T').length}/${gr[k].length} tr?ng � gi� �${HESO[k]})</small></div>`;
    Object.keys(rows).sort((a, b) => a - b).forEach(r => {
      h += `<div class="row"><span class="rn">${r}</span>`;
      rows[r].forEach((g, i) => {
        const c = ['seat', g.tt === 'T' ? 'free' : g.tt === 'H' ? 'held' : 'taken'];
        if (o.sel === g.so && g.tt === 'T') c.push('sel'); if (o.cur === g.so) c.push('cur');
        const ok = (g.tt === 'T' && (!o.hang || o.hang === g.hang)) && !o.off;
        h += `<button class="${c.join(' ')}" data-so="${g.so}" title="${g.so} � ${HANG[g.hang]}" ${ok ? '' : 'disabled'}>${g.so.replace(/^\d+/, '')}</button>`;
        if (i === per / 2 - 1) h += '<span class="aisle"></span>';
      });
      h += '</div>';
    });
    h += '</div>';
  }
  return h + '</div><div class="legend"><span><i style="background:#dcfce7;border:1px solid #86efac"></i>Tr?ng</span><span><i style="background:#fef3c7;border:1px solid #fcd34d"></i>�ang gi?</span><span><i style="background:#e2e8f0"></i>�? �?t</span><span><i style="background:#1d4ed8"></i>�ang ch?n</span></div>';
}

/* ---------------- �?T V� ---------------- */
async function pDat() {
  const all = await api('chuyenbay', { sapxep: 'gio' }); S.flights = all.chuyenBay;
  if (!S.ma || !S.flights.find(c => c.ma === S.ma)) S.ma = S.flights.length ? S.flights[0].ma : null;
  S.sel = null; S.hold = null;
  const dl = user.vaiTro === 'DL';
  $('#panel').innerHTML = `<div class="two"><div class="card"><h2>Ch?n chuy?n &amp; gh?</h2>
      <label>Chuy?n bay</label><select id="fl">${S.flights.map(c => `<option value="${c.ma}">${c.ma} � ${c.di}?${c.den} � ${hm(c.gioDi)} � ${vnd(c.gia)}</option>`).join('')}</select>
      <div id="map" style="margin-top:12px"></div></div>
    <div class="card"><h2>${dl ? '�?t v� h? kh�ch' : '�?t v�'}</h2><div id="box"></div></div></div>`;
  $('#fl').value = S.ma; $('#fl').onchange = guard(async () => { S.ma = $('#fl').value; S.sel = null; await loadMap(); });
  await loadMap();
}
async function loadMap() {
  if (!S.ma) { $('#map').innerHTML = '<div class="empty">Ch�a c� chuy?n bay</div>'; return; }
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
  if (!g) { box.innerHTML = `<div class="empty">B?m v�o m?t gh? tr?ng �? ch?n</div>` + (dl ? '' : ''); return; }
  box.innerHTML = `<div class="kv"><span>Chuy?n</span><b>${S.cb.ma} � ${S.cb.di} ? ${S.cb.den}</b><span>Kh?i h�nh</span><b>${hm(S.cb.gioDi)}</b>
      <span>Gh?</span><b>${g.so}</b><span>H?ng v�</span><b>${tagH(g.hang)}</b></div>
    <div class="mut">Gi� d? ki?n (gi� c� b?n � ${HESO[g.hang]})</div><div class="big">${vnd(S.cb.gia * HESO[g.hang])}</div>
    ${dl ? `<div style="margin-top:10px"><label>M? h�nh kh�ch</label><input id="mk" value="HK01"></div>` : ''}
    <label style="margin-top:10px"><input type="checkbox" id="fast" style="width:auto"> Demo nhanh: gi? ch? ch? <b>15 gi�y</b> (xem h?t h?n)</label>
    <div class="acts"><button class="b" id="gc">Gi? ch?</button></div>
    ${hood(['<em>QuanLyDatVe::giuCho()</em> ? <em>ChuyenBay::giuGhe()</em> ? t?o <em>GiuCho</em> (timeout)', 'gh? chuy?n <em>TRONG ? GIU_CHO</em> trong <em>SoDoGhe</em>'])}`;
  $('#gc').onclick = guard(async () => {
    const d = await post('giucho', { maCB: S.ma, soGhe: S.sel, maKhach: dl ? $('#mk').value : '', giay: $('#fast').checked ? 15 : '' });
    S.hold = { ...d, until: Date.now() + d.conLai * 1000, maKhach: dl ? $('#mk').value : '' }; toast('�? gi? gh? ' + d.soGhe); await loadMap(); startTimer();
  });
}
function startTimer() { clearInterval(S.timer); S.timer = setInterval(tick, 250); tick(); }
function tick() {
  const e = $('#cd'); if (!e || !S.hold) return clearInterval(S.timer);
  const left = Math.max(0, Math.ceil((S.hold.until - Date.now()) / 1000));
  e.textContent = `${Math.floor(left / 60)}:${String(left % 60).padStart(2, '0')}`;
  if (left === 0) { $('#st').innerHTML = '<span class="tag t-wait">H?t th?i gian gi? ch?</span>'; }
}
function holdBox() {
  const h = S.hold;
  $('#box').innerHTML = `<div class="kv"><span>Chuy?n</span><b>${h.maCB}</b><span>Gh?</span><b>${h.soGhe}</b><span>H?ng v�</span><b>${tagH(h.hang)}</b></div>
    <div class="mut">Th?i gian gi? ch? c?n l?i</div><div class="cd" id="cd">--:--</div><div id="st"><span class="tag t-ok">�ang gi? ch?</span></div>
    <div class="acts"><button class="b" id="pay">Thanh to�n</button><button class="b g" id="cancel">H?y gi? ch?</button></div>
    ${hood(['<em>GiuCho::conHieuLuc()</em> ��?c ki?m tra khi thanh to�n', 'h?t h?n ? <em>LoiGiuChoHetHan</em>, gh? t? tr? v? <em>TRONG</em> (<em>ChuyenBay::donDepGiuCho()</em>)'])}`;
  $('#pay').onclick = guard(async () => {
    const id = S.hold.holdId; clearInterval(S.timer);
    try { const d = await post('thanhtoan', { holdId: id }); S.hold = null; S.sel = null; toast('�?t v� th�nh c�ng ' + d.ve.ma); await loadMap(); $('#box').innerHTML = ticketHtml(d.ve, true) + '<div class="acts"><button class="b g" onclick="go(\'ve\')">Xem v�</button></div>'; }
    catch (e) { S.hold = null; S.sel = null; await loadMap(); throw e; }
  });
  $('#cancel').onclick = guard(async () => { clearInterval(S.timer); await post('huygiucho', { holdId: S.hold.holdId }); S.hold = null; S.sel = null; toast('�? h?y gi? ch?'); await loadMap(); });
  startTimer();
}

/* ---------------- V� ---------------- */
function chain(v) {
  const goc = v.hang === 'TG' ? 'VeThuongGia' : v.hang === 'PT' ? 'VePhoThong' : 'VeTietKiem';
  return hood([`<em>VeFactory::taoVe()</em> ? <b>${goc}</b>`,
    v.dichVu.length ? `Decorator: <em>${esc(v.doiTuong)}</em>` : 'Kh�ng c� d?ch v? ? kh�ng b?c Decorator']);
}
function ticketHtml(v, withHood, acts) {
  const dv = v.dichVu.map(d => `<div>+ ${esc(d)}</div>`).join('');
  return `<div class="ticket ${v.hang === 'TG' ? 'tg' : v.hang === 'TK' ? 'tk' : ''}"><div class="h"><b>${v.ma}</b>${tagH(v.hang)}
      ${v.checkIn ? '<span class="tag t-ok">�? check-in</span>' : '<span class="tag t-wait">Ch�a check-in</span>'}<span class="sp"></span><b>${vnd(v.gia)}</b></div>
    <div><b>${v.maCB}</b> � ${v.di} ? ${v.den} � ${hm(v.gioDi)} ? ${hm(v.gioDen)}</div>
)HTML" +
        R"HTML(    <div class="mut">Gh? <b>${v.ghe}</b> � ${esc(v.hanhKhach.ten)} (${v.hanhKhach.ma}) � gi� v� g?c ${vnd(v.giaGoc)}</div>${dv ? `<div class="mut">${dv}</div>` : ''}
    ${withHood ? chain(v) : ''}${acts || ''}</div>`;
}
async function pVe() {
  const d = await api('ve'); const ro = user.vaiTro === 'AD';
  $('#panel').innerHTML = `<div class="card"><h2>${ro ? 'T?t c? v� trong h? th?ng' : user.vaiTro === 'DL' ? 'V� �? b�n' : 'V� c?a t�i'} (${d.ve.length})</h2>` +
    (d.ve.length ? d.ve.map(v => ticketHtml(v, true, ro ? '' : `<div class="acts"><button class="b s" data-c="${v.ma}">Check-in / �?i ch?</button>
      <button class="b g s" data-b="${v.ma}">Boarding pass</button><button class="b r s" data-h="${v.ma}">H?y v�</button></div>`)).join('') : '<div class="empty">Ch�a c� v� n�o</div>') + '</div>';
  $$('[data-c]').forEach(b => b.onclick = guard(() => checkInDlg(d.ve.find(v => v.ma === b.dataset.c))));
  $$('[data-b]').forEach(b => b.onclick = guard(async () => { const r = await api('boardingpass', { maVe: b.dataset.b }); modal(`<h2>Boarding pass ${r.ve.ma}</h2><pre class="bp">${esc(r.text)}</pre>${hood(['<em>QuanLyDatVe::inBoardingPass()</em> ? <em>VeMayBay::inBoardingPass()</em>' + (r.ve.hang === 'TG' ? ' (<em>VeThuongGia::coPhongCho()</em> qua <em>dynamic_cast</em>)' : '')])}<div class="acts"><button class="b" onclick="closeModal()">��ng</button></div>`); }));
  $$('[data-h]').forEach(b => b.onclick = guard(async () => { if (!confirm('H?y v� ' + b.dataset.h + '? Gh? s? ��?c tr? l?i.')) return; await post('huyve', { maVe: b.dataset.h }); toast('�? h?y v�'); await pVe(); }));
}
async function checkInDlg(v) {
  const d = await api('sodoghe', { ma: v.maCB }); let pick = v.ghe;
  const draw = () => { $('#cmap').innerHTML = seatMap(d.ghe.map(g => g.so === v.ghe ? { ...g, tt: 'T' } : g), { sel: pick, cur: v.ghe, hang: v.hang });
    $$('#cmap .seat').forEach(b => b.onclick = () => { pick = b.dataset.so; draw(); $('#pk').textContent = pick; }); };
  modal(`<h2>Check-in v� ${v.ma} � chuy?n ${v.maCB}</h2><p class="mut">Gh? hi?n t?i <b>${v.ghe}</b> (vi?n cam). Ch?n gh? m?i c�ng h?ng ${tagH(v.hang)} ho?c gi? nguy�n. Check-in m? tr�?c gi? bay 24 gi?, ��ng tr�?c 1 gi?.</p>
    <div id="cmap"></div><p>Gh? �? ch?n: <b id="pk">${pick}</b></p>
    ${hood(['<em>QuanLyDatVe::checkIn(maVe, soGheMoi)</em>', 'v� Ti?t ki?m kh�ng ��?c �?i ch?: <em>dynamic_cast&lt;VeTietKiem*&gt;(veGoc())->choPhepDoiVe()</em>'])}
    <div class="acts"><button class="b" id="ok">X�c nh?n check-in</button><button class="b g" onclick="closeModal()">��ng</button></div>`);
  draw();
  $('#ok').onclick = guard(async () => { await post('checkin', { maVe: v.ma, soGhe: pick }); closeModal(); toast('Check-in th�nh c�ng, gh? ' + pick); await pVe(); });
}

/* ---------------- H�NH TR?NH (BUILDER + DECORATOR) ---------------- */
async function pHT() {
  const all = await api('chuyenbay', { sapxep: 'gio' }); S.flights = all.chuyenBay; const dl = user.vaiTro === 'DL';
  $('#panel').innerHTML = `<div class="two"><div class="card"><h2>D?ng h�nh tr?nh (Builder)</h2>
    <div class="grid"><div><label>H?ng v�</label><select id="hg"><option value="TG">Th��ng gia (�2.5)</option><option value="PT" selected>Ph? th�ng (�1.0)</option><option value="TK">Ti?t ki?m (�0.8)</option></select></div>
    ${dl ? '<div><label>M? h�nh kh�ch</label><input id="mk" value="HK01"></div>' : ''}</div>
    <h3>C�c ch?ng bay</h3><div id="legs"></div>
    <div class="grid" style="margin-top:8px"><div><label>Th�m ch?ng</label><select id="addsel">${S.flights.map(c => `<option value="${c.ma}">${c.ma} � ${c.di}?${c.den} � ${hm(c.gioDi)}</option>`).join('')}</select></div><div><button class="b g" id="add">+ Th�m ch?ng</button></div></div>
    <h3>D?ch v? gia t�ng (Decorator)</h3>
    <div class="grid"><div><label>H�nh l? th�m (kg) � 50.000 �/kg</label><input id="kg" type="number" min="0" value="0"></div>
    <div><label>Su?t �n (�? tr?ng = kh�ng)</label><input id="mon" placeholder="vd: C�m g�"></div><div><label>Gi� su?t �n (�)</label><input id="gia" type="number" min="0" value="150000"></div></div>
    <div class="acts"><button class="b" id="dat">�?t h�nh tr?nh</button></div></div>
    <div class="card"><h2>K?t qu?</h2><div id="out"><div class="empty">Ch?n c�c ch?ng r?i b?m ��?t h�nh tr?nh�</div></div></div></div>`;
  $('#hg').value = S.hanh;
  const drawLegs = () => { $('#legs').innerHTML = S.legs.length ? S.legs.map((m, i) => { const c = S.flights.find(x => x.ma === m); return `<div class="leg"><b>${i + 1}.</b> ${m} ${c ? c.di + ' ? ' + c.den + ' <span class="mut">' + hm(c.gioDi) + '</span>' : ''}<span class="sp"></span><button class="b g s" data-x="${i}">B?</button></div>`; }).join('') : '<div class="mut">Ch�a c� ch?ng n�o</div>';
    $$('#legs [data-x]').forEach(b => b.onclick = () => { S.legs.splice(b.dataset.x, 1); drawLegs(); }); };
  drawLegs();
  $('#add').onclick = () => { S.legs.push($('#addsel').value); drawLegs(); };
  $('#dat').onclick = guard(async () => {
    S.hanh = $('#hg').value;
    const d = await post('hanhtrinh', { hang: S.hanh, chang: S.legs.join(','), kg: $('#kg').value, mon: $('#mon').value, gia: $('#gia').value, maKhach: dl ? $('#mk').value : '' });
    $('#out').innerHTML = d.ve.map(v => ticketHtml(v, true)).join('') + `<div class="kv"><span>T?ng c?ng</span><b class="big">${vnd(d.tong)}</b></div>` +
      hood(['<em>HanhTrinhBuilder</em>.chonHanhKhach().chonHang().themChang()�themHanhLy().themSuatAn().<em>build()</em> ? <em>HanhTrinh</em>', '<em>QuanLyDatVe::datVeTheoHanhTrinh()</em>: m?i ch?ng <em>VeFactory</em> t?o v�, r?i b?c <em>VeCoHanhLy</em> / <em>VeCoSuatAn</em>']);
    S.legs = []; drawLegs(); toast('�? �?t ' + d.ve.length + ' v�');
  });
}

/* ---------------- TH?NG K� ---------------- */
async function pTK() {
  const d = await api('thongke'); const mx = Math.max(1, ...Object.values(d.loaiVe));
  $('#panel').innerHTML = `<div class="stat"><div class="card"><small>Doanh thu v�</small><div class="big">${vnd(d.doanhThu)}</div></div>
    <div class="card"><small>S? v� �? b�n</small><div class="big">${d.soVe}</div></div><div class="card"><small>S? chuy?n bay</small><div class="big">${d.soChuyen}</div></div>
    ${d.hoaHong !== undefined ? `<div class="card"><small>Hoa h?ng �?i l? (5%)</small><div class="big">${vnd(d.hoaHong)}</div></div>` : ''}</div>
    <div class="card" style="margin-top:16px"><h2>V� theo h?ng</h2>${Object.entries(d.loaiVe).map(([k, n]) => `<div style="display:grid;grid-template-columns:110px 1fr;gap:10px;margin-bottom:8px;align-items:center"><span>${esc(k)}</span><div><div class="bar" style="width:${Math.max(4, n / mx * 100)}%">${n}</div></div></div>`).join('')}
    ${hood(['<em>QuanLyDatVe::thongKeLoaiVe()</em> � d�ng <em>dynamic_cast</em> tr�n <em>veGoc()</em> n�n v� �? b?c Decorator v?n �?m ��ng h?ng', '<em>QuanLyDatVe::tinhDoanhThu()</em> � <em>DaiLy::getTongHoaHong()</em>'])}</div>`;
}

/* ---------------- ADMIN ---------------- */
async function pQly() {
  const d = await api('chuyenbay', { sapxep: 'gio' });
  const f = (id, title, body) => `<div class="card"><h3 style="margin-top:0">${title}</h3><form id="${id}" class="grid">${body}<div><button class="b">Th�m</button></div></form></div>`;
  $('#panel').innerHTML = `<div class="card"><h2>Danh s�ch chuy?n bay (${d.chuyenBay.length})</h2><div id="tb"></div></div>
    <div class="two">${f('fcb', 'Th�m chuy?n bay', `<div><label>M? chuy?n</label><input name="ma" required></div><div><label>T?</label><select name="di">${apOpts(false)}</select></div><div><label>�?n</label><select name="den">${apOpts(false)}</select></div>
      <div><label>M�y bay</label><select name="mb">${meta.mayBay.map(m => `<option>${esc(m)}</option>`).join('')}</select></div><div><label>Gi? �i</label><input type="datetime-local" name="gioDi" required></div><div><label>Gi? �?n</label><input type="datetime-local" name="gioDen" required></div><div><label>Gi� c� b?n (�)</label><input type="number" name="gia" value="1000000" required></div>`)}
    <div>${f('fsb', 'Th�m s�n bay', '<div><label>M?</label><input name="ma" required></div><div><label>T�n</label><input name="ten"></div><div><label>Th�nh ph?</label><input name="tp"></div>')}
    ${f('fmb', 'Th�m m�y bay', '<div><label>S? hi?u</label><input name="soHieu" required></div><div><label>Model</label><input name="model"></div><div><label>Gh? TG</label><input type="number" name="tg" value="8"></div><div><label>Gh? PT</label><input type="number" name="pt" value="24"></div><div><label>Gh? TK</label><input type="number" name="tk" value="12"></div>')}</div></div>`;
  $('#tb').innerHTML = flightRows(d.chuyenBay, c => `<button class="b r s" data-d="${c.ma}">X�a</button>`);
  $$('[data-d]').forEach(b => b.onclick = guard(async () => { if (!confirm('X�a chuy?n ' + b.dataset.d + '?')) return; await post('admin/xoachuyenbay', { ma: b.dataset.d }); toast('�? x�a'); await pQly(); }));
  const hook = (id, path, msg) => { $('#' + id).onsubmit = guard(async e => { e.preventDefault(); await post(path, Object.fromEntries(new FormData(e.target))); toast(msg); meta = await api('meta'); await pQly(); }); };
  hook('fcb', 'admin/chuyenbay', '�? th�m chuy?n bay'); hook('fsb', 'admin/sanbay', '�? th�m s�n bay'); hook('fmb', 'admin/maybay', '�? th�m m�y bay');
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
std::string tenHang(HangGhe h) { return h == HangGhe::THUONG_GIA ? "Thuong gia" : h == HangGhe::PHO_THONG ? "Pho thong" : "Tiet kiem"; }
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
    if (gioDen <= gioDi) throw LoiNghiepVu("Gio den phai sau gio di");
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
    {"POST /api/login", hLogin},           {"GET /api/me", hMe},                  {"POST /api/logout", hLogout},
    {"GET /api/meta", hMeta},              {"GET /api/chuyenbay", hChuyenBay},    {"GET /api/noichuyen", hNoiChuyen},
    {"GET /api/sodoghe", hSoDoGhe},        {"POST /api/giucho", hGiuCho},         {"POST /api/huygiucho", hHuyGiuCho},
    {"POST /api/thanhtoan", hThanhToan},   {"POST /api/hanhtrinh", hHanhTrinh},   {"GET /api/ve", hVe},
    {"POST /api/checkin", hCheckIn},       {"GET /api/boardingpass", hBoardingPass}, {"POST /api/huyve", hHuyVe},
    {"GET /api/thongke", hThongKe},        {"POST /api/admin/sanbay", hThemSanBay}, {"POST /api/admin/maybay", hThemMayBay},
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
            std::cout << "Da nap du lieu tu thu muc data/ (dung --reset de bat dau lai tu du lieu mau)\n";
        } else {
            nhapDuLieuMau(*S.ql, *S.nd);
            luu();
            std::cout << "Lan chay dau: da tao du lieu mau.\n";
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
bool batDauTuConsole(QuanLyChuyenBay& ql, QuanLyDatVe& dv, QuanLyNguoiDung& nd, FileRepository& repo, int cong) {
    if (S.dangChay.load()) {
        std::cout << "Web dang chay san tai http://localhost:" << cong << "\n";
        return true;
    }
    S.ganTuConsole(ql, dv, nd, repo);
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
              << "  Mo trinh duyet:  http://localhost:" << cong << "\n"
              << "  Moi thao tac dat ve / them chuyen tren console se hien ngay tren web.\n"
              << "  Tai khoan: AD01/admin123  DL01/daily123  HK01/hk123  HK02/hk456\n\n";
    return true;
}
}  // namespace web
#endif  // BAT_WEB_DEMO


// ======================================================================
// HAM MAIN
// ======================================================================
int main(int argc, char** argv) {
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
            std::cout << "Da nap du lieu tu thu muc data/\n";
        } else {
            nhapDuLieuMau(qlChuyenBay, qlNguoiDung);
            std::cout << "Lan chay dau: da tao du lieu mau.\n"
                      << "  Tai khoan: AD01/admin123 (Admin), DL01/daily123 (Dai ly), HK01/hk123, HK02/hk456 (Hanh khach)\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Loi nap du lieu: " << e.what() << "\n";
        return 1;
    }
    Menu menu(qlChuyenBay, qlDatVe, qlNguoiDung, repo);
    menu.chay();
    return 0;
}
