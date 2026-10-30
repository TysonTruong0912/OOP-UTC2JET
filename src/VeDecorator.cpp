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

namespace {
const VeMayBay& kiemTraVe(const std::unique_ptr<VeMayBay>& v) {
    if (!v) throw LoiNghiepVu("Decorator can mot ve goc hop le");
    return *v;
}
}

// Decorator vua IS-A (ke thua VeMayBay) vua HAS-A (chua ve goc trong "ve").
VeDecorator::VeDecorator(std::unique_ptr<VeMayBay> v)
    : VeMayBay(kiemTraVe(v).getMaVe(), v->getSoGhe(), v->getChuyenBay(), v->getHanhKhach()),
      ve(std::move(v)) {
    if (ve->isDaCheckIn()) danhDauCheckIn();
}

const VeMayBay* VeDecorator::veGoc() const { return ve->veGoc(); }

std::string VeDecorator::loaiVe() const { return ve->loaiVe(); }
