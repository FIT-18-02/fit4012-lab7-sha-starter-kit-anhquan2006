#ifndef FIT4012_SHA_STRUCTURE_H
#define FIT4012_SHA_STRUCTURE_H

#include <array>
#include <cstdint>
#include <string>

// Định nghĩa các kiểu dữ liệu cơ bản
using byte = std::uint8_t;   // 8-bit (1 byte)
using word = std::uint32_t;  // 32-bit (4 bytes - 1 word)

// Cấu trúc bổ trợ để quản lý block dữ liệu trong SHA-256
struct SHA256_Block {
    std::array<byte, 64> bytes; // 1 Block SHA-256 tiêu chuẩn = 64 bytes (512 bits)
};

// Giá trị khởi tạo H(0): 32 bit đầu tiên của phần phân số từ căn bậc hai của 8 số nguyên tố đầu tiên.
static constexpr std::array<word, 8> H0_INITIAL = {
    0x6a09e667U, 0xbb67ae85U, 0x3c6ef372U, 0xa54ff53aU,
    0x510e527fU, 0x9b05688cU, 0x1f83d9abU, 0x5be0cd19U
};

// Hằng số vòng K[0..63]: 32 bit đầu tiên của phần phân số từ căn bậc ba của 64 số nguyên tố đầu tiên.
static constexpr std::array<word, 64> K256 = {
    0x428a2f98U, 0x71374491U, 0xb5c0fbcfU, 0xe9b5dba5U,
    0x3956c25bU, 0x59f111f1U, 0x923f82a4U, 0xab1c5ed5U,
    0xd807aa98U, 0x12835b01U, 0x243185beU, 0x550c7dc3U,
    0x72be5d74U, 0x80deb1feU, 0x9bdc06a7U, 0xc19bf174U,
    0xe49b69c1U, 0xefbe4786U, 0x0fc19dc6U, 0x240ca1ccU,
    0x2de92c6fU, 0x4a7484aaU, 0x5cb0a9dcU, 0x76f988daU,
    0x983e5152U, 0xa831c66dU, 0xb00327c8U, 0xbf597fc7U,
    0xc6e00bf3U, 0xd5a79147U, 0x06ca6351U, 0x14292967U,
    0x27b70a85U, 0x2e1b2138U, 0x4d2c6dfcU, 0x53380d13U,
    0x650a7354U, 0x766a0abbU, 0x81c2c92eU, 0x92722c85U,
    0xa2bfe8a1U, 0xa81a664bU, 0xc24b8b70U, 0xc76c51a3U,
    0xd192e819U, 0xd6990624U, 0xf40e3585U, 0x106aa070U,
    0x19a4c116U, 0x1e376c08U, 0x2748774cU, 0x34b0bcb5U,
    0x391c0cb3U, 0x4ed8aa4aU, 0x5b9cca4fU, 0x682e6ff3U,
    0x748f82eeU, 0x78a5636fU, 0x84c87814U, 0x8cc70208U,
    0x90befffaU, 0xa4506cebU, 0xbef9a3f7U, 0xc67178f2U
};

// --- Các hàm thao tác Bit chức năng cơ bản ---

// Xoay phải (Rotate Right)
inline word ROTR(word x, unsigned int n) {
    return (x >> n) | (x << (32U - n));
}

// Dịch phải (Shift Right)
inline word SHR(word x, unsigned int n) {
    return x >> n;
}

// Hàm Choose: Nếu bit của x là 1 thì chọn y, ngược lại chọn z
inline word Ch(word x, word y, word z) {
    return (x & y) ^ (~x & z);
}

// Hàm Majority: Trả về bit xuất hiện nhiều hơn (đa số) giữa x, y, z
inline word Maj(word x, word y, word z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

// Các hàm biến đổi Sigma lớn và nhỏ phục vụ cho các vòng lặp nén SHA-256
inline word Sigma0_256(word x) {
    return ROTR(x, 2U) ^ ROTR(x, 13U) ^ ROTR(x, 22U);
}

inline word Sigma1_256(word x) {
    return ROTR(x, 6U) ^ ROTR(x, 11U) ^ ROTR(x, 25U);
}

inline word sigma0_256(word x) {
    return ROTR(x, 7U) ^ ROTR(x, 18U) ^ SHR(x, 3U);
}

inline word sigma1_256(word x) {
    return ROTR(x, 17U) ^ ROTR(x, 19U) ^ SHR(x, 10U);
}

#endif // FIT4012_SHA_STRUCTURE_H
