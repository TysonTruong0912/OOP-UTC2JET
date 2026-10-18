// LoiDinhDangFile.cpp — P5_MenuFileWeb
#include "LoiDinhDangFile.h"
#include <algorithm>
#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

LoiDinhDangFile::LoiDinhDangFile(std::string msg) : LoiNghiepVu(std::move(msg)) {}