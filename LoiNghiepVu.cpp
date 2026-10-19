// LoiNghiepVu.cpp — P5_MenuFileWeb
#include "LoiNghiepVu.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

LoiNghiepVu::LoiNghiepVu(std::string msg) : std::runtime_error(msg) {}