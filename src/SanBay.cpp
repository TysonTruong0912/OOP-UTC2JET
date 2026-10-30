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

SanBay::SanBay(std::string ma, std::string ten, std::string thanhPho)
    : maSB(std::move(ma)), ten(std::move(ten)), thanhPho(std::move(thanhPho)) {}

std::string SanBay::getMaSB() const { return maSB; }
std::string SanBay::getTen() const { return ten; }
std::string SanBay::getThanhPho() const { return thanhPho; }
