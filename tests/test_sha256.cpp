#include <iostream>
#include <string>
#include <cassert>
#include "sha256.hpp"

void run_test(const std::string& input, const std::string& expected) {
    std::string result = sha256(input);
    if (result == expected) {
        std::cout << "[PASS] Input: \"" << input << "\"\n";
    } else {
        std::cout << "[FAIL] Expected: " << expected << "\n       but got:  " << result << "\n";
        assert(false);
    }
}

int main() {
    std::cout << "Running SHA-256 cryptographic test vectors...\n\n";
    
    // Official Test Vector 1: Empty String
    run_test("", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

    // Official Test Vector 2: "abc"
    run_test("abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

    // Official Test Vector 3: "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
    run_test("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");

    std::cout << "\nAll cryptographic tests passed flawlessly!\n";
    return 0;
}
