#include "sha256_lib.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

namespace {

std::string normalize_hex(std::string text) {

    std::transform(
        text.begin(),
        text.end(),
        text.begin(),
        [](unsigned char ch) {
            return static_cast<char>(std::tolower(ch));
        });

    return text;
}

void print_usage(const char* program_name) {

    std::cout
        << "Usage:\n"
        << "  " << program_name
        << " <file_path>\n"
        << "  " << program_name
        << " <file_path> <expected_sha256_hex>\n";
}

bool verify_file_integrity(const std::string& file_path,
                           const std::string& expected_hash) {

    const std::string actual_hash =
        sha256::calculate_sha256_file(file_path);

    return (actual_hash == normalize_hex(expected_hash));
}

} // namespace

int main(int argc, char* argv[]) {

    try {

        if (argc != 2 && argc != 3) {

            print_usage(argv[0]);
            return 1;
        }

        const std::string file_path =
            argv[1];

        const std::string actual_hash =
            sha256::calculate_sha256_file(file_path);

        // ONLY PRINT HASH
        if (argc == 2) {

            std::cout
                << actual_hash
                << std::endl;

            return 0;
        }

        const std::string expected_hash =
            normalize_hex(argv[2]);

        // VERIFY SUCCESS
        if (actual_hash == expected_hash) {

            std::cout
                << "[PASS] File integrity verified\n";

            std::cout
                << "SHA-256: "
                << actual_hash
                << std::endl;

            return 0;
        }

        // VERIFY FAILED
        std::cout
            << "[FAIL] File integrity check failed\n";

        std::cout
            << "Expected: "
            << expected_hash
            << std::endl;

        std::cout
            << "Actual  : "
            << actual_hash
            << std::endl;

        return 1;

    } catch (const std::exception& ex) {

        std::cerr
            << "[ERROR] "
            << ex.what()
            << std::endl;

        return 1;
    }
}
//vh
