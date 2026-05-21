#ifndef FIT4012_SHA256_LIB_H
#define FIT4012_SHA256_LIB_H

#include "structure.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace sha256 {

inline word bytes_to_word_be(const byte* bytes_ptr) {

    return
        (static_cast<word>(bytes_ptr[0]) << 24U) |
        (static_cast<word>(bytes_ptr[1]) << 16U) |
        (static_cast<word>(bytes_ptr[2]) << 8U)  |
        (static_cast<word>(bytes_ptr[3]));
}

inline void word_to_bytes_be(word value,
                             byte* bytes_ptr) {

    bytes_ptr[0] =
        static_cast<byte>((value >> 24U) & 0xFFU);

    bytes_ptr[1] =
        static_cast<byte>((value >> 16U) & 0xFFU);

    bytes_ptr[2] =
        static_cast<byte>((value >> 8U) & 0xFFU);

    bytes_ptr[3] =
        static_cast<byte>(value & 0xFFU);
}

inline std::string bytes_to_hex_string(
    const std::array<byte, 32>& bytes) {

    std::ostringstream oss;

    oss << std::hex
        << std::setfill('0');

    for (byte b : bytes) {

        oss << std::setw(2)
            << static_cast<unsigned int>(b);
    }

    return oss.str();
}

inline std::vector<byte> string_to_bytes(
    const std::string& text) {

    return std::vector<byte>(
        text.begin(),
        text.end()
    );
}

inline std::vector<byte> read_file_bytes(
    const std::string& file_path) {

    std::ifstream input(
        file_path,
        std::ios::binary
    );

    if (!input.is_open()) {

        throw std::runtime_error(
            "Cannot open file: " + file_path
        );
    }

    return std::vector<byte>(
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>()
    );
}

inline void sha256_transform(
    word state[8],
    const byte block[64]) {

    word W[64];

    for (int t = 0; t < 16; ++t) {

        W[t] =
            bytes_to_word_be(block + (t * 4));
    }

    for (int t = 16; t < 64; ++t) {

        W[t] =
            sigma1_256(W[t - 2]) +
            W[t - 7] +
            sigma0_256(W[t - 15]) +
            W[t - 16];
    }

    word a = state[0];
    word b = state[1];
    word c = state[2];
    word d = state[3];
    word e = state[4];
    word f = state[5];
    word g = state[6];
    word h = state[7];

    for (int t = 0; t < 64; ++t) {

        const word T1 =
            h +
            Sigma1_256(e) +
            Ch(e, f, g) +
            K256[t] +
            W[t];

        const word T2 =
            Sigma0_256(a) +
            Maj(a, b, c);

        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

inline std::array<byte, 32> calculate_sha256_bytes(
    const std::vector<byte>& input_bytes) {

    word current_hash_state[8];

    for (std::size_t i = 0;
         i < H0_INITIAL.size();
         ++i) {

        current_hash_state[i] =
            H0_INITIAL[i];
    }

    std::vector<byte> padded =
        input_bytes;

    const std::uint64_t original_bit_length =
        static_cast<std::uint64_t>(
            input_bytes.size()
        ) * 8ULL;

    // Append bit 1
    padded.push_back(0x80U);

    // Pad with zeros until length ≡ 56 mod 64
    while ((padded.size() % 64U) != 56U) {

        padded.push_back(0x00U);
    }

    // Append original length (64-bit big endian)
    for (int shift = 56;
         shift >= 0;
         shift -= 8) {

        padded.push_back(
            static_cast<byte>(
                (original_bit_length >> shift) & 0xFFU
            )
        );
    }

    // Process each 512-bit block
    for (std::size_t offset = 0;
         offset < padded.size();
         offset += 64U) {

        sha256_transform(
            current_hash_state,
            padded.data() + offset
        );
    }

    std::array<byte, 32> hash_result{};

    for (int i = 0; i < 8; ++i) {

        word_to_bytes_be(
            current_hash_state[i],
            hash_result.data() + (i * 4)
        );
    }

    return hash_result;
}

inline std::string calculate_sha256_hex(
    const std::vector<byte>& input_bytes) {

    return bytes_to_hex_string(
        calculate_sha256_bytes(input_bytes)
    );
}

inline std::string calculate_sha256_string(
    const std::string& text) {

    return calculate_sha256_hex(
        string_to_bytes(text)
    );
}

inline std::string calculate_sha256_file(
    const std::string& file_path) {

    return calculate_sha256_hex(
        read_file_bytes(file_path)
    );
}

} // namespace sha256

#endif // FIT4012_SHA256_LIB_H
//vh
