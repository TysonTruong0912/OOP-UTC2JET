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

MayBay::MayBay(std::string soHieu, std::string model, int gheTG, int ghePT, int gheTK)
    : soHieu(std::move(soHieu)), model(std::move(model)),
      soGheThuongGia(gheTG), soGhePhoThong(ghePT), soGheTietKiem(gheTK) {}

std::string MayBay::getSoHieu() const { return soHieu; }

int MayBay::getSoGhe(HangGhe h) const {
    switch (h) {
        case HangGhe::THUONG_GIA: return soGheThuongGia;
        case HangGhe::PHO_THONG:  return soGhePhoThong;
        case HangGhe::TIET_KIEM:  return soGheTietKiem;
    }
    return 0;
}

int MayBay::tongSoGhe() const { return soGheThuongGia + soGhePhoThong + soGheTietKiem; }
