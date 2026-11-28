// 23 · Bitset — standard implementation class
// n bits packed into 64-bit words: 1 bit per flag instead of 1 byte per bool
// ops: set(i), clear(i), test(i), count() = num of bits set
//
// clarify:
// size -> fixed at construction, all bits start at 0
// i out of [0, n) -> never, the caller checks (follow-up 1)
// count -> no builtin popcount

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::vector;
using std::uint64_t;

// core idea: bit i lives in words[i / 64] at bit pos i % 64; a mask 1ULL << pos picks it
// T: set / clear / test O(1), count O(num of words + num of set bits), S: O(n / 64)
class Bitset {
    vector<uint64_t> words; // bit i = bit (i % 64) of words[i / 64]

public:
    // round up: 65 bits need 2 words
    explicit Bitset(int n) : words((n + 63) / 64, 0) {}

    // 1ULL, not 1: 1 << 40 overflows int
    void set(int i) { words[i / 64] |= 1ULL << (i % 64); }

    void clear(int i) { words[i / 64] &= ~(1ULL << (i % 64)); }

    bool test(int i) const { return ((words[i / 64] >> (i % 64)) & 1) != 0; }

    int count() const {
        int total = 0;
        for (uint64_t word : words) {
            // word & (word - 1) drops the lowest set bit
            while (word != 0) {
                word &= word - 1;
                total++;
            }
        }
        return total;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Bitset filled(100);
    check(filled.count() == 0 && !filled.test(0), "start-empty");

    filled.set(3);
    filled.set(63);
    filled.set(64);
    filled.set(3);
    check(filled.test(3) && !filled.test(4) && filled.count() == 3, "set-test-count");
    check(filled.test(63) && filled.test(64) && !filled.test(65), "word-edge");

    filled.clear(63);
    check(!filled.test(63) && filled.test(64) && filled.count() == 2, "clear");
}

// follow-up:
// 1. i out of range -> keep n as a member; guard i < 0 || i >= n in each op
// 2. faster count -> __builtin_popcountll per word, or a 256-entry table per byte
// 3. first set bit -> first nonzero word, then __builtin_ctzll(word)
// 4. flip(i) -> words[i / 64] ^= mask
// 5. and / or of two bitsets -> word by word
// 6. vector<bool> -> the std version of this packing; its operator[] returns a proxy
