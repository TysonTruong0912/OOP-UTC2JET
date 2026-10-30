// DaiLy.cpp
#include "DaiLy.h"


DaiLy::DaiLy(std::string ma, std::string ten, std::string mk, double tyLe)
    : NguoiDung(std::move(ma), std::move(ten), std::move(mk)),
      tyLeHoaHong(tyLe), tongHoaHong(0.0),
      hhBanVeHomNay(0.0), hhThiTruongHomNay(0.0), hhHomQua(0.0),
      ngayHoaHong(ngayHeThongHomNay()) {
    dongBoNgayThuc();
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
