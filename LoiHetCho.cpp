// LoiHetCho.cpp — P5_MenuFileWeb
#include "LoiHetCho.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

LoiHetCho::LoiHetCho(std::string msg) : LoiNghiepVu(std::move(msg)) {}