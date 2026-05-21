#!/usr/bin/env bash
# Thiết lập cấu hình nghiêm ngặt cho bash script:
# -e: Dừng script ngay nếu có bất kỳ lệnh g++ nào bị biên dịch lỗi
# -u: Dừng script nếu dùng biến chưa khai báo
# -o pipefail: Giúp bắt lỗi chính xác trong các đường ống lệnh
set -euo pipefail

# --- GIAI ĐOẠN 1: Tiến hành biên dịch các file mã nguồn sang file thực thi ---

# Biên dịch chương trình thuật toán lõi SHA-256
g++ -std=c++17 -Wall -Wextra -pedantic sha_procedure.cpp -o sha256

# Biên dịch bài toán Kiểm tra tính toàn vẹn (Cần đính kèm file logic băm sha_procedure.cpp)
g++ -std=c++17 -Wall -Wextra -pedantic file_integrity.cpp sha_procedure.cpp -o file_integrity

# Biên dịch bài toán Băm mật khẩu cơ bản (Cần đính kèm file logic băm sha_procedure.cpp)
g++ -std=c++17 -Wall -Wextra -pedantic password_hash.cpp sha_procedure.cpp -o password_hash

# Biên dịch bài toán Băm mật khẩu có muối (Cần đính kèm file logic băm sha_procedure.cpp)
g++ -std=c++17 -Wall -Wextra -pedantic salted_password_hash.cpp sha_procedure.cpp -o salted_password_hash


# --- GIAI ĐOẠN 2: Kiểm tra sự tồn tại và quyền thực thi (-x) của các file sau khi build ---

[[ -x ./sha256 ]] || { echo "[FAIL] Missing sha256 executable"; exit 1; }
[[ -x ./file_integrity ]] || { echo "[FAIL] Missing file_integrity executable"; exit 1; }
[[ -x ./password_hash ]] || { echo "[FAIL] Missing password_hash executable"; exit 1; }
[[ -x ./salted_password_hash ]] || { echo "[FAIL] Missing salted_password_hash executable"; exit 1; }

# Nếu vượt qua toàn bộ các bước kiểm tra trên mà không bị crash giữa chừng
echo "[PASS] SHA programs compile successfully."
