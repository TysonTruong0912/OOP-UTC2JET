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

HanhLy::HanhLy(double kg, double donGiaKg)
    : DichVuGiaTang("HL", "Hanh ly"), soKg(kg), donGiaKg(donGiaKg) {}

double HanhLy::tinhPhi() const { return soKg * donGiaKg; }

// Dinh dang co dinh "Hanh ly <kg>kg (<don gia>/kg)": FileRepository doc lai chuoi nay khi nap ve tu file.
std::string HanhLy::moTa() const {
    std::ostringstream os;
    os << std::setprecision(10) << "Hanh ly " << soKg << "kg (" << donGiaKg << "/kg)";
    return os.str();
}
