@echo off
cd /d "%~dp0"
g++ -std=c++17 -O1 BTL_OOP.cpp -o datve.exe -lws2_32
if errorlevel 1 (echo LOI BIEN DICH & pause & exit /b 1)
echo === CHAY CONSOLE ===
datve.exe
pause
