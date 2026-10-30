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

GiuCho::GiuCho(ChuyenBay* cb, std::string ghe, HanhKhach* hk, std::chrono::seconds timeout)
    : chuyenBay(cb), soGhe(std::move(ghe)), hanhKhach(hk),
      hetHan(std::chrono::steady_clock::now() + timeout) {}

ChuyenBay* GiuCho::getChuyenBay() const { return chuyenBay; }
std::string GiuCho::getSoGhe() const { return soGhe; }
HanhKhach* GiuCho::getHanhKhach() const { return hanhKhach; }
bool GiuCho::conHieuLuc() const { return std::chrono::steady_clock::now() < hetHan; }
