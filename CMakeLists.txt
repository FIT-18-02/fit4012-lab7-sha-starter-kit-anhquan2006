cmake_minimum_required(VERSION 3.16)
project(fit4012_lab7_sha LANGUAGES CXX)

# Cấu hình tiêu chuẩn C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Tự động thêm thư mục hiện tại vào include path để các file .cpp tìm thấy thư mục/file .h dễ dàng
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 1. Tạo file thực thi chính cho lõi SHA-256 (Dùng để chạy test thuật toán nền tảng)
add_executable(sha256 sha_procedure.cpp)

# 2. Tạo file thực thi cho bài toán Kiểm tra tính toàn vẹn file
# Cần build chung với sha_procedure.cpp để sử dụng hàm băm lõi
add_executable(file_integrity file_integrity.cpp sha_procedure.cpp)

# 3. Tạo file thực thi cho bài toán Băm mật khẩu cơ bản
add_executable(password_hash password_hash.cpp sha_procedure.cpp)

# 4. Tạo file thực thi cho bài toán Băm mật khẩu có thêm muối (Salted)
add_executable(salted_password_hash salted_password_hash.cpp sha_procedure.cpp)
