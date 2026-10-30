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

NguoiDung::NguoiDung(std::string ma, std::string ten, std::string mk)
    : maND(std::move(ma)), hoTen(std::move(ten)), matKhau(std::move(mk)) {}

NguoiDung::~NguoiDung() = default;

std::string NguoiDung::getMaND() const { return maND; }
std::string NguoiDung::getHoTen() const { return hoTen; }
bool NguoiDung::dangNhap(std::string mk) const { return mk == matKhau; }
