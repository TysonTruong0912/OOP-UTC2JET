// main.cpp — P5

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
            capNhatGioChuyenTheoThoiGianThuc(qlChuyenBay);
            xuatMaTranNoiChuyen(qlChuyenBay);
        } else {
            nhapDuLieuMau(qlChuyenBay, qlNguoiDung);
            xuatMaTranNoiChuyen(qlChuyenBay);
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
