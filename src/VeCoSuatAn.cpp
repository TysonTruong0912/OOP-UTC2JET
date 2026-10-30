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

VeCoSuatAn::VeCoSuatAn(std::unique_ptr<VeMayBay> v, SuatAn sa)
    : VeDecorator(std::move(v)), suatAn(sa) {}

double VeCoSuatAn::tinhGia() const { return ve->tinhGia() + suatAn.tinhPhi(); }

std::string VeCoSuatAn::moTa() const { return ve->moTa() + " + " + suatAn.moTa(); }
