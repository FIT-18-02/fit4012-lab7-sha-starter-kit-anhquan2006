#!/usr/bin/env bash
# Thiết lập cấu hình nghiêm ngặt cho bash script:
# -e: Dừng script ngay nếu có bất kỳ lệnh nào bị lỗi (exit code khác 0)
# -u: Dừng script nếu sử dụng một biến chưa được định nghĩa trước
# -o pipefail: Trả về mã lỗi của lệnh thất bại đầu tiên trong chuỗi đường ống (pipe)
set -euo pipefail

# Thực hiện biên dịch chương trình core sha256 thông qua Makefile
make sha256 >/dev/null

# Khai báo các giá trị băm SHA-256 chuẩn quốc tế (Known Vector) để làm mốc đối chiếu
EMPTY_EXPECTED="e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
ABC_EXPECTED="ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"

# Chạy chương trình sha256 vừa biên dịch để băm chuỗi rỗng và chuỗi "abc" thực tế
EMPTY_ACTUAL=$(./sha256 --hash-string "")
ABC_ACTUAL=$(./sha256 --hash-string "abc")

# --- KIỂM TRA 1: Đối chiếu kết quả của chuỗi rỗng ---
[[ "$EMPTY_ACTUAL" == "$EMPTY_EXPECTED" ]] || {
  echo "[FAIL] Empty string vector mismatch"
  echo "expected: $EMPTY_EXPECTED"
  echo "actual  : $EMPTY_ACTUAL"
  exit 1
}

# --- KIỂM TRA 2: Đối chiếu kết quả của chuỗi "abc" ---
[[ "$ABC_ACTUAL" == "$ABC_EXPECTED" ]] || {
  echo "[FAIL] abc vector mismatch"
  echo "expected: $ABC_EXPECTED"
  echo "actual  : $ABC_ACTUAL"
  exit 1
}

# --- KIỂM TRA 3: Kích hoạt tính năng tự kiểm tra (Self-test) tích hợp sẵn trong code C++ ---
./sha256 --self-test >/dev/null

# Nếu vượt qua toàn bộ các bước so khớp trên, thuật toán SHA-256 lõi của ông đã chuẩn 100%
echo "[PASS] SHA-256 known answer tests passed."
