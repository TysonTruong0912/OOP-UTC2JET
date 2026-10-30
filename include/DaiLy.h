#pragma once
#include "Common.h"
#include "NguoiDung.h"

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
