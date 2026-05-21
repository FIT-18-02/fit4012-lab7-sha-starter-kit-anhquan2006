#include "sha256_lib.h"

#include <iostream>
#include <string>

namespace {

bool test_sha256_procedural(const std::string& input, const std::string& expected_hex) {
    const std::string actual_hex = sha256::calculate_sha256_string(input);
    const bool ok = (actual_hex == expected_hex);

    std::cout << (ok ? "[PASS] " : "[FAIL] ")
              << "Input: \"" << input << "\"\n"
              << "Expected: " << expected_hex << "\n"
              << "Actual  : " << actual_hex << "\n";

    return ok;
}

bool run_self_tests() {
    bool ok = true;

    ok = test_sha256_procedural(
        "",
        "e3b0c44298fc1c149afbf4c8996fb924"
        "27ae41e4649b934ca495991b7852b855") && ok;

    ok = test_sha256_procedural(
        "abc",
        "ba7816bf8f01cfea414140de5dae2223"
        "b00361a396177a9cb410ff61f20015ad") && ok;

    ok = test_sha256_procedural(
        "hello FIT4012 SHA",
        "e942ae0ecb44fe48e1490144162fc64e"
        "9c3c6c399bb4e2d686532195cdc3eae6") && ok;

    if (ok) {
        std::cout << "\nAll SHA-256 self-tests passed.\n";
    } else {
        std::cout << "\nSome SHA-256 self-tests failed.\n";
    }

    return ok;
}

void print_usage(const char* program_name) {
    std::cout << "Usage:\n"
              << "  " << program_name << " --self-test\n"
              << "  " << program_name << " --hash-string \"abc\"\n"
              << "  " << program_name << " --hash-file sample.txt\n";
}

} // namespace

int main(int argc, char* argv[]) {
    try {

        if (argc >= 2) {

            const std::string mode = argv[1];

            if (mode == "--self-test") {
                return run_self_tests() ? 0 : 1;
            }

            else if (mode == "--hash-string") {

                if (argc < 3) {
                    std::cerr << "Missing input string.\n";
                    return 1;
                }

                std::cout << sha256::calculate_sha256_string(argv[2]) << std::endl;
                return 0;
            }

            else if (mode == "--hash-file") {

                if (argc < 3) {
                    std::cerr << "Missing file path.\n";
                    return 1;
                }

                std::cout << sha256::calculate_sha256_file(argv[2]) << std::endl;
                return 0;
            }

            else {
                print_usage(argv[0]);
                return 1;
            }
        }

        std::string input;

        std::cout << "Nhap chuoi can bam SHA-256: ";
        std::getline(std::cin, input);

        std::cout << "SHA-256: "
                  << sha256::calculate_sha256_string(input)
                  << std::endl;

        return 0;

    } catch (const std::exception& ex) {

        std::cerr << "[ERROR] " << ex.what() << std::endl;
        return 1;
    }
}
