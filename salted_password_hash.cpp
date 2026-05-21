#include "sha256_lib.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace {

constexpr const char* DEFAULT_PASSWORD_FILE = "password.hash";

void print_usage(const char* program_name) {

    std::cout
        << "Usage:\n"
        << "  " << program_name
        << " register <password> [password_file]\n"
        << "  " << program_name
        << " login <password> [password_file]\n"
        << "  " << program_name
        << " hash-with-salt <password> <salt_hex>\n";
}

std::string bytes_to_hex(const std::vector<byte>& bytes) {

    std::ostringstream oss;

    oss << std::hex << std::setfill('0');

    for (byte b : bytes) {
        oss << std::setw(2)
            << static_cast<int>(b);
    }

    return oss.str();
}

std::string generate_random_salt(std::size_t size = 16) {

    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 255);

    std::vector<byte> salt(size);

    for (byte& b : salt) {
        b = static_cast<byte>(dist(rd));
    }

    return bytes_to_hex(salt);
}

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

std::string salted_hash(const std::string& salt_hex,
                        const std::string& password) {

    return sha256::calculate_sha256_string(
        normalize_hex(salt_hex) + password
    );
}

std::string choose_file(int argc, char* argv[]) {

    if (argc >= 4) {
        return std::string(argv[3]);
    }

    return std::string(DEFAULT_PASSWORD_FILE);
}

bool save_salted_password(const std::string& password,
                          const std::string& password_file) {

    std::ofstream output(password_file);

    if (!output.is_open()) {
        return false;
    }

    const std::string salt =
        generate_random_salt();

    const std::string hash =
        salted_hash(salt, password);

    output << salt << ":" << hash;

    return true;
}

bool verify_salted_password(const std::string& password,
                            const std::string& password_file) {

    std::ifstream input(password_file);

    if (!input.is_open()) {
        return false;
    }

    std::string line;
    std::getline(input, line);

    const std::size_t separator =
        line.find(':');

    if (separator == std::string::npos) {
        return false;
    }

    const std::string salt =
        line.substr(0, separator);

    const std::string stored_hash =
        line.substr(separator + 1);

    const std::string current_hash =
        salted_hash(salt, password);

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

        if (password.empty()) {
            std::cerr
                << "[ERROR] Empty password is not allowed\n";

            return 1;
        }

        // HASH WITH SALT
        if (mode == "hash-with-salt") {

            if (argc != 4) {
                print_usage(argv[0]);
                return 1;
            }

            const std::string salt =
                argv[3];

            std::cout
                << salted_hash(salt, password)
                << std::endl;

            return 0;
        }

        const std::string password_file =
            choose_file(argc, argv);

        // REGISTER
        if (mode == "register") {

            if (save_salted_password(
                    password,
                    password_file)) {

                std::cout
                    << "[PASS] Salted password registered successfully\n";

                return 0;
            }

            std::cerr
                << "[ERROR] Cannot write password file\n";

            return 1;
        }

        // LOGIN
        else if (mode == "login") {

            if (verify_salted_password(
                    password,
                    password_file)) {

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
