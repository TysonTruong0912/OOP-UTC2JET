// VeFactory.cpp
#include "VeFactory.h"

std::unique_ptr<VeMayBay> VeFactory::taoVe(HangGhe h, std::string maVe, std::string soGhe, ChuyenBay* cb,
                                           HanhKhach* hk) {
    switch (h) {
        case HangGhe::THUONG_GIA: return std::make_unique<VeThuongGia>(std::move(maVe), std::move(soGhe), cb, hk);
        case HangGhe::PHO_THONG:  return std::make_unique<VePhoThong>(std::move(maVe), std::move(soGhe), cb, hk);
        case HangGhe::TIET_KIEM:  return std::make_unique<VeTietKiem>(std::move(maVe), std::move(soGhe), cb, hk);
    }
    throw LoiNghiepVu("Hang ghe khong hop le");
}
