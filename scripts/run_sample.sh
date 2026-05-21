#!/usr/bin/env bash
# Thiết lập cấu hình nghiêm ngặt cho bash script:
# -e: Dừng kịch bản ngay lập tức nếu có bất kỳ lệnh nào bị lỗi (exit code khác 0)
# -u: Dừng kịch bản nếu phát hiện biến chưa được định nghĩa
# -o pipefail: Bắt lỗi chính xác từ lệnh đầu tiên bị thất bại trong chuỗi pipe
set -euo pipefail

# Kích hoạt biên dịch toàn bộ các file thực thi trong project thông qua quy tắc 'all' của Makefile
make all

echo "== 1. Known answer tests =="
# Chạy tính năng tự kiểm tra (Self-test) tích hợp sẵn trong chương trình lõi sha256
./sha256 --self-test

echo "== 2. Hash string =="
# Thử nghiệm băm một chuỗi văn bản cụ thể để xem chuỗi hash hexa đầu ra
./sha256 --hash-string "hello FIT4012 SHA"

echo "== 3. File integrity =="
# Tạo file văn bản mẫu 'sample.txt' để test tính toàn vẹn dữ liệu
printf "FIT4012 SHA file integrity sample\n" > sample.txt

# Tính toán mã hash chuẩn của file mẫu vừa tạo
EXPECTED_HASH=$(./sha256 --hash-file sample.txt)

# Chạy thử chương trình file_integrity lần đầu với file nguyên bản (kết quả mong đợi là SUCCESS)
./file_integrity sample.txt "$EXPECTED_HASH"

# Cố tình ghi thêm dữ liệu "tampered" vào cuối file nhằm thay đổi nội dung dữ liệu gốc
printf "tampered\n" >> sample.txt

# Kiểm tra lại tính toàn vẹn sau khi file đã bị chỉnh sửa dữ liệu (Tamper)
if ./file_integrity sample.txt "$EXPECTED_HASH"; then
  # Nếu chương trình vẫn báo Pass (trả về exit code 0) tức là logic kiểm tra đang bị sai
  echo "[FAIL] Tamper case should fail"
  exit 1
else
  # Nếu chương trình chặn lại và báo lỗi (exit code khác 0), tức là đã phát hiện chỉnh sửa thành công
  echo "[PASS] Tamper case detected"
fi

echo "== 4. Password hash =="
# Test Đăng ký tài khoản mới: Băm mật khẩu thường và lưu vào file 'test_password.hash'
./password_hash register "fit4012-demo-password" test_password.hash

# Test Đăng nhập: Đăng nhập thử với mật khẩu chính xác (kết quả mong đợi là SUCCESS)
./password_hash login "fit4012-demo-password" test_password.hash

# Test Đăng nhập lỗi: Cố tình đăng nhập bằng mật khẩu sai
if ./password_hash login "wrong-password" test_password.hash; then
  # Nếu đăng nhập sai chuỗi mật khẩu mà hệ thống vẫn cho qua thì báo FAIL
  echo "[FAIL] Wrong password should fail"
  exit 1
else
  # Hệ thống từ chối truy cập thành công
  echo "[PASS] Wrong password rejected"
fi

echo "== 5. Salted password hash =="
# Test Đăng ký nâng cao: Đăng ký CÙNG MỘT MẬT KHẨU vào hai file lưu trữ khác nhau
./salted_password_hash register "fit4012-demo-password" test_password_salted_1.hash
./salted_password_hash register "fit4012-demo-password" test_password_salted_2.hash

# Thử nghiệm đăng nhập lại trên tài khoản đầu tiên bằng cơ chế Salted Login
./salted_password_hash login "fit4012-demo-password" test_password_salted_1.hash

# Kiểm tra tính độc lập của Muối (Salt): So sánh xem hai file hash có khác nhau hay không
if cmp -s test_password_salted_1.hash test_password_salted_2.hash; then
  # Nếu trùng nhau tức là cơ chế sinh Salt ngẫu nhiên chưa hoạt động đúng -> FAIL
  echo "[FAIL] Same password should produce different salted records"
  exit 1
else
  # Nếu hai file khác nhau hoàn toàn dù chung mật khẩu gốc -> Đạt chuẩn bảo mật
  echo "[PASS] Same password produced different salted records"
fi
