// LoiGiuChoHetHan.cpp — P5_MenuFileWeb
#include "LoiGiuChoHetHan.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

LoiGiuChoHetHan::LoiGiuChoHetHan(std::string msg) : LoiNghiepVu(std::move(msg)) {}