@echo off
chcp 65001 >nul
cd /d "%~dp0"
echo [1/2] Bien dich (AirlineSystem + web)...
g++ -std=c++17 -O1 -Iinclude src/AirlineSystem.cpp -o datve.exe -lws2_32
if errorlevel 1 (
  echo LOI bien dich. Can cai MinGW g++ va thu lai.
  pause
  exit /b 1
)
echo [2/2] Chay datve.exe
echo Web: trong menu chon mo web, hoac mo datve.html / http://localhost:8080
datve.exe
pause
