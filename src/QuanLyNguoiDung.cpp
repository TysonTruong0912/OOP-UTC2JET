#include "AirlineSystem.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <stdexcept>
#include <regex>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;
namespace fs = std::filesystem;

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
