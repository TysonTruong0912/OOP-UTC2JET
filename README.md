# Demo Vercel — đầy đủ chức năng UI

## Chức năng
- Đăng nhập Admin / Đại lý / HK, đăng ký HK (validate CCCD, SĐT, email)
- Tìm chuyến thẳng, nối chuyến + Đặt cả hành trình
- Đặt vé: hạng TG/PT/TK, ghế, hành lý, suất ăn, đặt hộ ĐL
- Vé + check-in, thống kê, so sánh đại lý
- Admin: thêm sân bay, thêm chuyến, xem ĐL/hoa hồng

## Deploy
Upload folder này lên vercel.com → Deploy → mở link.

## C++ OOP
Không chạy trên Vercel. Dùng DT04_DatVe trên máy.


## Một link — máy bạn full, máy khác chỉ khách

Cùng một URL Vercel cho mọi người.

**Máy của bạn (full Admin + Đại lý):**
1. Lần đầu: Đăng nhập form bằng **AD01** + mật khẩu Admin (không cần logo).
2. Trình duyệt máy bạn tự nhớ → **mọi lần sau** chỉ cần mở link là thấy đủ menu Quản trị / Đại lý.
3. Không cần bật lại, không cần thêm `?host=` trên link gửi đi.

**Máy/ĐT người khác (cùng link):**
- Không có đánh dấu máy chủ → chỉ Đăng nhập / Đăng ký hành khách.
- Không đăng nhập được tài khoản Đại lý.
- Không thấy menu Admin / Đại lý.

(Tuỳ chọn dự phòng: nhấp logo 5 lần + mã `DT04HOST`.)



## Đồng bộ đa thiết bị (chuyến bay)

Trạng thái chuyến (trễ/hoãn/hủy/sửa/thêm) được đẩy lên `/api/sync` và các máy khác **poll mỗi 5 giây**.

### Cấu hình trên Vercel (bắt buộc để sync thật giữa 2 máy)

1. Vercel Dashboard → Project → **Storage** → tạo **Blob** store → connect vào project.
2. Env tự có `BLOB_READ_WRITE_TOKEN`.
3. (Tuỳ chọn) thêm `SYNC_SECRET` — mặc định client dùng `SkyportSync2024` (đổi cả env và trong `js/app.js` nếu cần).
4. Redeploy.

**Giới hạn:** độ trễ khoảng vài giây (polling 5s). Không có Blob token thì API vẫn chạy nhưng `configured: false`, mỗi máy chỉ giữ localStorage riêng.

### Kiểm tra nhanh

- Máy A (admin): đổi trạng thái chuyến.
- Máy B (khách): trong ~5s banner cảnh báo / danh sách cập nhật, có toast thông báo.
