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

VeCoHanhLy::VeCoHanhLy(std::unique_ptr<VeMayBay> v, HanhLy hl)
    : VeDecorator(std::move(v)), hanhLy(hl) {}

double VeCoHanhLy::tinhGia() const { return ve->tinhGia() + hanhLy.tinhPhi(); }

std::string VeCoHanhLy::moTa() const { return ve->moTa() + " + " + hanhLy.moTa(); }
