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

VeTietKiem::VeTietKiem(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk)
    : VeMayBay(std::move(ma), std::move(ghe), cb, hk) {}

double VeTietKiem::tinhGia() const { return chuyenBay->getGiaCoBan() * HE_SO; }

std::string VeTietKiem::loaiVe() const { return "Tiết kiệm"; }

bool VeTietKiem::choPhepDoiVe() const { return false; }
