#pragma once
/**
 * AirlineSystem.h — toàn bộ khai báo enum + 34 class
 * (tách từ PHAN 1 + PHAN 2 của BTL_OOP.cpp)
 *
 * Chia module khi làm Jira: có thể tách tiếp file này thành
 * NguoiDung.h, ChuyenBay.h, Ve.h, ... theo đúng bảng 5 thành viên.
 */
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
  #include <netdb.h>
  #include <unistd.h>
  #include <unistd.h>
#endif

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>

#if defined(_WIN32)
#include <windows.h>
static void batUtf8Console() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    // bat che do UTF-8 cho iostream neu co the
    std::system("chcp 65001 >nul");
}
#else
static void batUtf8Console() {}
#endif


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
#include <cctype>
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
std::vector<std::string> layDiaChiLan();
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
    double getHoaHongHomNay() const;       // ban ve + thi truong (hien thi)
    double getHoaHongHomQua() const;
    double getHoaHongBanVeHomNay() const;
    double getHoaHongThiTruongHomNay() const;
    double getChenhLechVoiHomQua() const;  // hom nay - hom qua (+ tang, - giam)
    std::string getNgayHoaHong() const;
    double getTyLeHoaHong() const { return tyLeHoaHong; }
    void setTongHoaHong(double t);
    void setHoaHongHomNay(double h, const std::string& ngay);
    void setHoaHongHomQua(double h);
    void dongBoNgayThuc();   // cap nhat theo ngay + gio he thong (thi truong tu dong)
    void hienMenu() override;
    std::string vaiTro() const override;
private:
    double tyLeHoaHong{};
    double tongHoaHong{};
    double hhBanVeHomNay{};       // chi tu dat ve qua dai ly
    double hhThiTruongHomNay{};   // uoc luong thi truong theo ngay/gio (khong can nhap)
    double hhHomQua{};            // tong HH ngay hom qua (de so sanh)
    std::string ngayHoaHong;
    double hoaHongHomNayHienThi() const { return hhBanVeHomNay + hhThiTruongHomNay; }
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


// ----- Ghế -----
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
    void dichGio(std::chrono::seconds delta);
    void datGioBay(time_point gioDiMoi, time_point gioDenMoi);  // neo gio that theo ngay
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
