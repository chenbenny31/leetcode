// bitset: n bits packed into 64-bit words, 1 bit per flag instead of 1 byte per bool
// ops: set(i), clear(i), test(i), count() = num of bits set

// usage:
// g++ -std=c++17 -Wall -Wextra 22-bitset.cpp -o a && ./a
//
// Bitset filled(n);  filled.set(i);  filled.test(i);  filled.count();

// clarify / assumptions
// size n is fixed at construction, all bits start at 0
// i out of [0, n) -> set / clear do nothing, test returns false

// bit-ops:
// i / 64 -> which word, i % 64 -> bit pos inside the word
// mask = 1ULL << pos -> only that bit, 1ULL because 1 << 40 overflows int
// word |= mask -> set, word &= ~mask -> clear, word & mask -> test
// word &= word - 1 -> drop the lowest set bit

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

using std::uint64_t;
using std::cout;
using std::string;
using std::vector;

class Bitset {
private:
    static constexpr int WORD_BITS = 64;
    vector<uint64_t> words; // bit i lives in words[i / 64] at bit pos i % 64
    int size; // num of bits
public:
    // round up: 65 bits need 2 words
    Bitset(int n) : words((n + WORD_BITS - 1) / WORD_BITS, 0), size(n) {}

    // T: O(1) for set / clear / test, S: O(n / 64) words
    void set(int i) {
        if (i < 0 || i >= size) { return; }
        uint64_t mask = 1ULL << (i % WORD_BITS);
        words[i / WORD_BITS] |= mask;
    }

    void clear(int i) {
        if (i < 0 || i >= size) { return; }
        uint64_t mask = 1ULL << (i % WORD_BITS);
        words[i / WORD_BITS] &= ~mask;
    }

    bool test(int i) {
        if (i < 0 || i >= size) { return false; }
        uint64_t mask = 1ULL << (i % WORD_BITS);
        return (words[i / WORD_BITS] & mask) != 0;
    }

    // each loop round drops one set bit, T: O(num of words + num of set bits)
    int count() {
        int total = 0;
        for (uint64_t word : words) {
            while (word != 0) {
                word &= word - 1;
                total++;
            }
        }
        return total;
    }
};

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // order ids 0..99, bit = 1 when the order is filled
    Bitset filled(100);
    check("start-empty", filled.count() == 0 && !filled.test(0));

    filled.set(3);
    filled.set(63);
    filled.set(64);
    check("set-test", filled.test(3) && !filled.test(4));
    check("word-edge", filled.test(63) && filled.test(64) && !filled.test(65));
    check("count", filled.count() == 3);

    filled.set(3);
    check("set-twice", filled.count() == 3);

    filled.clear(63);
    check("clear", !filled.test(63) && filled.test(64) && filled.count() == 2);

    // test invalid cases
    filled.set(100);
    filled.set(-1);
    check("out-of-range", !filled.test(100) && filled.count() == 2);
}
