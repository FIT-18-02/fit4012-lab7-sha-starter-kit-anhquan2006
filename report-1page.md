# FIT4012 Lab 7 - Báo cáo 1 trang: SHA-256

## 1. Mục tiêu / Objective

Bài thực hành nhằm mục tiêu phân tích sâu cơ chế hoạt động của hàm băm bảo mật SHA-256, bao gồm cấu trúc đệm dữ liệu Merkle-Damgård (thêm byte 0x80, bù 0x00 và chèn độ dài 64-bit Big Endian) và 64 vòng nén lõi. Qua đó, áp dụng thuật toán vào 3 kịch bản thực tế: kiểm tra tính toàn vẹn của tệp dữ liệu, cơ chế xác thực mật khẩu cơ bản và nâng cao an toàn mật khẩu bằng kỹ thuật băm có muối (Salted Hashing).

## 2. Cách làm / Approach

Quá trình thực hiện bài thực hành cá nhân bao gồm:
- Phân tích mã nguồn lõi trong thư viện `sha256_lib.h` và cấu trúc các hàm xử lý bit lõi trong `structure.h`.
- Khảo sát chương trình chính `sha_procedure.cpp` để băm chuỗi ký tự và tệp dữ liệu, xác thực thuật toán qua cơ chế tự kiểm tra (`--self-test`).
- Tối ưu hóa chương trình `file_integrity.cpp` để so sánh chuỗi băm thực tế của tệp với chuỗi băm mong muốn, trả về kết quả nghiêm ngặt nhằm phát hiện hành vi giả mạo.
- Tối ưu hóa chương trình `password_hash.cpp` phục vụ đăng ký và đăng nhập bằng cơ chế đối sánh chuỗi băm SHA-256 thô.
- Phát triển chương trình `salted_password_hash.cpp` ứng dụng cơ chế sinh muối ngẫu nhiên (16 byte hex), kết hợp với mật khẩu gốc trước khi băm để lưu trữ bản ghi an toàn theo định dạng `salt:hash`.
- Chuẩn hóa toàn bộ định dạng đầu ra của các ứng dụng ngoại vi về hai trạng thái `[PASS]` hoặc `[FAIL]` nhằm đáp ứng tuyệt đối điều kiện kiểm thử của hệ thống Autograder.

## 3. Kết quả / Result

Các minh chứng và kết quả thu được từ hệ thống kiểm thử:
- Hash của chuỗi `abc`: `ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad`
- Hash của file mẫu trước khi sửa: Khớp hoàn toàn với chuỗi hash gốc được tạo ra từ chương trình chính.
- Kết quả kiểm tra file sau khi sửa nội dung: Hệ thống trả về `[FAIL]`, phát hiện tệp đã bị thay đổi tính toàn vẹn (Tamper Detected).
- Kết quả đăng nhập với mật khẩu đúng: Hệ thống trả về `[PASS]` và cho phép truy cập thành công.
- Kết quả đăng nhập với mật khẩu sai: Hệ thống trả về `[FAIL]` và từ chối phiên đăng nhập.
- Hai bản ghi `salt:hash` của cùng một mật khẩu có giống nhau không? **Không giống nhau**. Do mỗi lần đăng ký hệ thống tự động tạo ra một chuỗi muối ngẫu nhiên hoàn toàn khác nhau, dẫn đến chuỗi đầu vào của hàm băm thay đổi và tạo ra hai kết quả băm hoàn toàn khác biệt.

## 4. Kết luận / Conclusion

- **SHA-256 giúp phát hiện thay đổi dữ liệu như thế nào?** SHA-256 sở hữu thuộc tính an toàn "Hiệu ứng thác đổ" (Avalanche effect). Chỉ cần thay đổi một bit cực nhỏ ở đầu vào (như thêm một dấu cách hay một ký tự ẩn), chuỗi băm 256-bit ở đầu ra sẽ biến đổi hoàn toàn và ngẫu nhiên, giúp hệ thống phát hiện ngay lập tức mọi hành vi sửa đổi tệp dù là tinh vi nhất.
- **Vì sao cần salt khi lưu hash mật khẩu?** Chuỗi muối (Salt) giúp vô hiệu hóa các cuộc tấn công tra cứu trước bằng bảng băm có sẵn (Rainbow Tables). Đồng thời, nếu hai người dùng sử dụng chung một mật khẩu phổ thông, việc áp dụng hai chuỗi muối khác nhau sẽ sinh ra hai chuỗi băm khác nhau trên cơ sở dữ liệu, ngăn chặn hacker suy đoán mật khẩu chéo giữa các tài khoản.
- **Vì sao SHA-256 demo trong lab chưa nên dùng trực tiếp cho hệ thống xác thực thật?** SHA-256 là hàm băm đa mục đích được thiết kế để tính toán với tốc độ cực nhanh. Trong hệ thống thực, hacker có thể tận dụng sức mạnh phần cứng chuyên dụng (GPU, ASIC) để thực hiện hàng tỷ phép thử băm một giây nhằm vét cạn (brute-force) mật khẩu. Hệ thống xác thực thật đòi hỏi các thuật toán chuyên dụng cho mật khẩu có cơ chế kiểm soát tài nguyên để kéo dài thời gian tính toán và tốn bộ nhớ như Argon2id, bcrypt hoặc scrypt.
