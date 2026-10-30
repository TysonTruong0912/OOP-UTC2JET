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

HangHangKhong::HangHangKhong(std::string ma, std::string ten)
    : maHang(std::move(ma)), ten(std::move(ten)) {}

std::string HangHangKhong::getMaHang() const { return maHang; }
std::string HangHangKhong::getTen() const { return ten; }
void HangHangKhong::themMayBay(MayBay* mb) { if (mb) dsMayBay.push_back(mb); }
void HangHangKhong::themChuyenBay(ChuyenBay* cb) { if (cb) dsChuyenBay.push_back(cb); }
int HangHangKhong::soChuyenBay() const { return static_cast<int>(dsChuyenBay.size()); }
