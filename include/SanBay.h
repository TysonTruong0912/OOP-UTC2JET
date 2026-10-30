#pragma once
#include "Common.h"

class SanBay {
public:
    SanBay(std::string ma, std::string ten, std::string thanhPho);
    std::string getMaSB() const;
    std::string getTen() const;
    std::string getThanhPho() const;
private:
    std::string maSB;
    std::string ten;
    std::string thanhPho;
};


// ----- MayBay -----
