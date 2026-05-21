CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2

SHA_TARGET := sha256
FILE_TARGET := file_integrity
PASS_TARGET := password_hash
SALT_TARGET := salted_password_hash

TARGETS := $(SHA_TARGET) $(FILE_TARGET) $(PASS_TARGET) $(SALT_TARGET)

.PHONY: all clean run test \
        hash-sample file-sample \
        password-sample salted-sample

all: $(TARGETS)

# =========================
# SHA-256 MAIN PROGRAM
# =========================
$(SHA_TARGET): sha_procedure.cpp sha256_lib.h structure.h
	$(CXX) $(CXXFLAGS) $< -o $@

# =========================
# FILE INTEGRITY
# =========================
$(FILE_TARGET): file_integrity.cpp sha256_lib.h structure.h
	$(CXX) $(CXXFLAGS) $< -o $@

# =========================
# PASSWORD HASH
# =========================
$(PASS_TARGET): password_hash.cpp sha256_lib.h structure.h
	$(CXX) $(CXXFLAGS) $< -o $@

# =========================
# SALTED PASSWORD HASH
# =========================
$(SALT_TARGET): salted_password_hash.cpp sha256_lib.h structure.h
	$(CXX) $(CXXFLAGS) $< -o $@

# =========================
# SAMPLE RUNS
# =========================
run: all
	bash scripts/run_sample.sh

hash-sample: $(SHA_TARGET)
	./$(SHA_TARGET) --hash-string "hello FIT4012 SHA"

file-sample: $(SHA_TARGET) $(FILE_TARGET)
	printf "FIT4012 SHA file integrity sample\n" > sample.txt
	./$(SHA_TARGET) --hash-file sample.txt
	./$(FILE_TARGET) sample.txt $$(./$(SHA_TARGET) --hash-file sample.txt)

password-sample: $(PASS_TARGET)
	./$(PASS_TARGET) register "fit4012-demo-password"
	./$(PASS_TARGET) login "fit4012-demo-password"

salted-sample: $(SALT_TARGET)
	./$(SALT_TARGET) register "fit4012-demo-password"
	./$(SALT_TARGET) login "fit4012-demo-password"

# =========================
# TESTS
# =========================
test: all
	bash tests/test_sha_compile.sh
	bash tests/test_known_vectors.sh
	bash tests/test_file_integrity_tamper.sh
	bash tests/test_password_hash.sh
	bash tests/test_salted_password.sh

# =========================
# CLEAN
# =========================
clean:
	rm -f $(TARGETS)
	rm -f *.o
	rm -f *.exe
	rm -f sample.txt
	rm -f password.hash
	rm -f test_password.hash
	rm -f test_password_salted_1.hash
	rm -f test_password_salted_2.hash
	rm -rf build
