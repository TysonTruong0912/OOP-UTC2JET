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
