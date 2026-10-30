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

Ghe::Ghe(std::string so, HangGhe h) : soGhe(std::move(so)), hang(h), trangThai(TrangThaiGhe::TRONG) {}

std::string Ghe::getSoGhe() const { return soGhe; }
HangGhe Ghe::getHang() const { return hang; }
TrangThaiGhe Ghe::getTrangThai() const { return trangThai; }
void Ghe::setTrangThai(TrangThaiGhe tt) { trangThai = tt; }
