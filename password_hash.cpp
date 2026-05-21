#include "sha256_lib.h"

#include <fstream>
#include <iostream>
#include <string>

namespace {

constexpr const char* DEFAULT_PASSWORD_FILE = "password.hash";

void print_usage(const char* program_name) {
    std::cout << "Usage:\n"
              << "  " << program_name << " register <password> [password_file]\n"
              << "  " << program_name << " login <password> [password_file]\n";
}

std::string choose_file(int argc, char* argv[]) {

    if (argc >= 4) {
        return std::string(argv[3]);
    }

    return std::string(DEFAULT_PASSWORD_FILE);
}

bool save_password_hash(const std::string& password,
                        const std::string& password_file) {

    std::ofstream output(password_file, std::ios::out);

    if (!output.is_open()) {
        return false;
    }

    const std::string hash =
        sha256::calculate_sha256_string(password);

    output << hash;

    return true;
}

bool verify_password(const std::string& password,
                     const std::string& password_file) {

    std::ifstream input(password_file, std::ios::in);

    if (!input.is_open()) {
        return false;
    }

    std::string stored_hash;
    std::getline(input, stored_hash);

    const std::string current_hash =
        sha256::calculate_sha256_string(password);

    return (stored_hash == current_hash);
}

} // namespace

int main(int argc, char* argv[]) {

    try {

        if (argc < 3 || argc > 4) {
            print_usage(argv[0]);
            return 1;
        }

        const std::string mode = argv[1];
        const std::string password = argv[2];
        const std::string password_file =
            choose_file(argc, argv);

        if (password.empty()) {
            std::cerr << "[ERROR] Empty password is not allowed\n";
            return 1;
        }

        // REGISTER
        if (mode == "register") {

            if (save_password_hash(password, password_file)) {

                std::cout
                    << "[PASS] Password registered successfully\n";

                return 0;
            }

            std::cerr
                << "[ERROR] Cannot write password file\n";

            return 1;
        }

        // LOGIN
        else if (mode == "login") {

            if (verify_password(password, password_file)) {

                std::cout
                    << "[PASS] Login successful\n";

                return 0;
            }

            std::cout
                << "[FAIL] Invalid password\n";

            return 1;
        }

        // INVALID MODE
        else {

            print_usage(argv[0]);
            return 1;
        }

    } catch (const std::exception& ex) {

        std::cerr
            << "[ERROR] "
            << ex.what()
            << std::endl;

        return 1;
    }
}
//vh
