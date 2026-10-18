// LoiChuyenBayKhongTonTai.cpp — P5_MenuFileWeb
#include "LoiChuyenBayKhongTonTai.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

LoiChuyenBayKhongTonTai::LoiChuyenBayKhongTonTai(std::string msg) : LoiNghiepVu(std::move(msg)) {}