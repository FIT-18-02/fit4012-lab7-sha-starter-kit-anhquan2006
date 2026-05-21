#!/usr/bin/env bash
# Thiết lập cấu hình nghiêm ngặt cho bash script:
# -e: Dừng script ngay nếu có bất kỳ lệnh nào bị lỗi (exit code khác 0)
# -u: Dừng script nếu sử dụng một biến chưa được định nghĩa trước
# -o pipefail: Trả về mã lỗi của lệnh thất bại đầu tiên trong chuỗi đường ống (pipe)
set -euo pipefail

# Thực hiện biên dịch chương trình salted_password_hash thông qua Makefile
make salted_password_hash >/dev/null

# Khai báo tên hai file dùng để lưu trữ hai bản ghi mật khẩu khác nhau
HASH_FILE_1="test_password_salted_1.hash"
HASH_FILE_2="test_password_salted_2.hash"
# Xóa các file cũ (nếu có) trước khi chạy test để đảm bảo môi trường sạch
rm -f "$HASH_FILE_1" "$HASH_FILE_2"

# --- GIAI ĐOẠN 1: Đăng ký hai tài khoản có CÙNG mật khẩu ---
# Mỗi lần chạy 'register', chương trình bắt buộc phải sinh ngẫu nhiên một chuỗi Salt mới
./salted_password_hash register "same-password" "$HASH_FILE_1" >/dev/null
./salted_password_hash register "same-password" "$HASH_FILE_2" >/dev/null

# Kiểm tra đảm bảo cả hai file hash đều đã được tạo thành công và không bị trống (-s)
[[ -s "$HASH_FILE_1" && -s "$HASH_FILE_2" ]] || {
  echo "[FAIL] Salted password hash files were not created"
  exit 1
}

# --- GIAI ĐOẠN 2: Kiểm tra tính ngẫu nhiên của Salt (Core Test) ---
# Sử dụng lệnh 'cmp -s' để so sánh từng byte của hai file dữ liệu.
# Nếu hai file giống hệt nhau (tức là không dùng salt hoặc salt bị trùng), lệnh cmp trả về 0 -> báo FAIL.
# Kết quả mong đợi là hai file phải KHÁC NHAU (tức là cmp trả về mã lỗi khác 0).
if cmp -s "$HASH_FILE_1" "$HASH_FILE_2"; then
  echo "[FAIL] Same password should not produce the same salted hash record"
  exit 1
fi

# --- GIAI ĐOẠN 3: Thử nghiệm Đăng nhập ĐÚNG mật khẩu ---
# Hệ thống cần phải đọc chuỗi Salt đã lưu trong HASH_FILE_1 ra, nối với "same-password" rồi băm lại để đối chiếu.
./salted_password_hash login "same-password" "$HASH_FILE_1" >/dev/null || {
  echo "[FAIL] Correct password should login successfully with saved salt"
  exit 1
}

# --- GIAI ĐOẠN 4: Thử nghiệm Đăng nhập SAI mật khẩu ---
# Cố tình đăng nhập bằng mật khẩu sai, hệ thống bắt buộc phải từ chối (exit code khác 0)
if ./salted_password_hash login "wrong password / sai mật khẩu" "$HASH_FILE_1" >/dev/null; then
  echo "[FAIL] Wrong password should be rejected in salted login"
  exit 1
fi

# Nếu vượt qua toàn bộ 4 giai đoạn trên, cơ chế Salted Hashing của ông đã đạt chuẩn bảo mật
echo "[PASS] Salted password test passed."
