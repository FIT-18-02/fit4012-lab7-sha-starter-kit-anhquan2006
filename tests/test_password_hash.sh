#!/usr/bin/env bash
# Thiết lập cấu hình nghiêm ngặt cho bash script:
# -e: Dừng script ngay nếu có bất kỳ lệnh nào bị lỗi (exit code khác 0)
# -u: Dừng script nếu sử dụng một biến chưa được định nghĩa trước
# -o pipefail: Trả về mã lỗi của lệnh thất bại đầu tiên trong chuỗi đường ống (pipe)
set -euo pipefail

# Thực hiện biên dịch chương trình password_hash thông qua Makefile
make password_hash >/dev/null

# Khai báo tên file dùng để lưu trữ dữ liệu mật khẩu sau khi băm
HASH_FILE="test_password.hash"
# Xóa file cũ (nếu có) trước khi bắt đầu test mới để tránh xung đột dữ liệu
rm -f "$HASH_FILE"

# --- GIAI ĐOẠN 1: Thử nghiệm Đăng ký tài khoản (Register) ---
# Gọi chương trình password_hash với tham số 'register', mật khẩu mẫu và file lưu trữ
./password_hash register "student-password-123" "$HASH_FILE" >/dev/null

# Kiểm tra xem file hash đã được tạo ra và có dung lượng lớn hơn 0 byte hay chưa (-s)
[[ -s "$HASH_FILE" ]] || { 
  echo "[FAIL] Password hash file was not created"
  exit 1 
}

# --- GIAI ĐOẠN 2: Thử nghiệm Đăng nhập ĐÚNG mật khẩu (Positive Login Test) ---
# Thực hiện đăng nhập thử với mật khẩu chính xác đã đăng ký ở trên
# Nếu chương trình trả về mã lỗi, block sau dấu || sẽ báo FAIL và dừng script
./password_hash login "student-password-123" "$HASH_FILE" >/dev/null || {
  echo "[FAIL] Correct password should login successfully"
  exit 1
}

# --- GIAI ĐOẠN 3: Thử nghiệm Đăng nhập SAI mật khẩu (Negative Login Test) ---
# Cố tình đăng nhập bằng một chuỗi mật khẩu sai hoàn toàn
# Vì mật khẩu sai, chương trình password_hash bắt buộc phải từ chối (exit code khác 0).
# Nếu chương trình chạy thành công (trả về 0), tức là hệ thống đang bị lỗi bảo mật nghiêm trọng.
if ./password_hash login "wrong password / sai mật khẩu" "$HASH_FILE" >/dev/null; then
  echo "[FAIL] Wrong password should be rejected"
  exit 1
fi

# Nếu vượt qua tất cả các bước trên một cách trơn tru, logic xử lý mật khẩu đã đúng
echo "[PASS] Password hash and wrong password negative test passed."
