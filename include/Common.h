#pragma once
/**
 * Common.h — include hệ thống + macro BAT_WEB_DEMO
 */
#define BAT_WEB_DEMO 1

#if BAT_WEB_DEMO && defined(_WIN32)
  #define NOMINMAX
  #define WIN32_LEAN_AND_MEAN
  #include <winsock2.h>
  #include <ws2tcpip.h>
#elif BAT_WEB_DEMO
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <netdb.h>
  #include <unistd.h>
#endif

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cctype>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

#if defined(_WIN32)
#include <windows.h>
inline void batUtf8Console() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::system("chcp 65001 >nul");
}
#else
inline void batUtf8Console() {}
#endif

namespace fs = std::filesystem;
using time_point = std::chrono::system_clock::time_point;

#include "Enums.h"
#include "Forward.h"
