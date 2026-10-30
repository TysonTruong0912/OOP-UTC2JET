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

SuatAn::SuatAn(std::string mon, double gia) : DichVuGiaTang("SA", "Suat an"), monAn(std::move(mon)), gia(gia) {}

double SuatAn::tinhPhi() const { return gia; }

// Dinh dang co dinh "Suat an: <mon> (<gia>)": FileRepository doc lai chuoi nay khi nap ve tu file.
std::string SuatAn::moTa() const {
    std::ostringstream os;
    os << std::setprecision(10) << "Suat an: " << monAn << " (" << gia << ")";
    return os.str();
}
