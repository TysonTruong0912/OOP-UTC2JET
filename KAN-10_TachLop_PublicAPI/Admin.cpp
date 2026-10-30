// Admin.cpp
#include "Admin.h"

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
