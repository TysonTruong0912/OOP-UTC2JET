#pragma once
#include "Common.h"
#include "ChuyenBay.h"
#include "HanhKhach.h"

class VeMayBay {
public:
    VeMayBay(std::string ma, std::string ghe, ChuyenBay* cb, HanhKhach* hk);
    virtual ~VeMayBay();
    std::string getMaVe() const;
    std::string getSoGhe() const;
    void setSoGhe(std::string ghe);
    ChuyenBay* getChuyenBay() const;
    HanhKhach* getHanhKhach() const;
    bool isDaCheckIn() const;
    void danhDauCheckIn();
    virtual double tinhGia() const = 0;
    virtual std::string loaiVe() const = 0;
    virtual std::string moTa() const;
    virtual const VeMayBay* veGoc() const;
    void inBoardingPass() const;
protected:
    std::string maVe;
    std::string soGhe;
    ChuyenBay* chuyenBay{};
    HanhKhach* hanhKhach{};
    std::chrono::system_clock::time_point ngayDat{};
    bool daCheckIn{};
};


// ----- VeThuongGia -----
