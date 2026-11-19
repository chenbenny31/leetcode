// memcpy / memmove: copy n bytes from src to dst and return dst
// memcpy may assume the two ranges don't overlap, memmove must work when they do

// usage:
// g++ -std=c++17 -Wall -Wextra 09-memmove.cpp -o a && ./a
//
// ranges never overlap -> myMemcpy
// ranges may overlap (shift data inside one buffer) -> myMemmove

// clarify / assumptions
// same signature as the std ones: void* dst, const void* src, size_t n
// caller guarantees both ranges are n bytes long, null is not checked
// byte by byte is fine; word-at-a-time / SIMD is the follow-up

// overlap:
// dst before src -> copy forward:  each src byte is read before it gets overwritten
// dst after src  -> copy backward: same reason, from the other end
//   src: [a b c d] e f      dst = src + 2, forward gives a b a b a b (wrong)
//   dst:     [a b c d]      backward gives a b a b c d (right)

#include <cstddef>
#include <cstdint>
#include <string>
#include <iostream>

using std::size_t;
using std::uint8_t;
using std::cout;
using std::string;

// forward copy, T: O(n), S: O(1)
void* myMemcpy(void* dst, const void* src, size_t n) {
    uint8_t* to = (uint8_t*)dst; // void* has no size, copy as bytes
    const uint8_t* from = (const uint8_t*)src;

    for (size_t i = 0; i < n; i++) {
        to[i] = from[i];
    }
    return dst;
}

// pick the direction that never overwrites a byte it still has to read
// T: O(n), S: O(1), no temp buffer
void* myMemmove(void* dst, const void* src, size_t n) {
    uint8_t* to = (uint8_t*)dst;
    const uint8_t* from = (const uint8_t*)src;

    if (to < from) { // dst before src -> forward
        for (size_t i = 0; i < n; i++) {
            to[i] = from[i];
        }
    } else if (to > from) { // dst after src -> backward
        for (size_t i = n; i > 0; i--) { // i - 1: size_t can't go below 0
            to[i - 1] = from[i - 1];
        }
    }
    return dst;
}

void test(const string& label, string text, int dstPos, int srcPos, int n,
          const string& want) {
    void* ret = myMemmove(&text[dstPos], &text[srcPos], n);
    bool ok = (text == want && ret == &text[dstPos]);

    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-overlap", "abcdef", 0, 3, 3, "defdef");
    test("dst-after-src", "abcdef", 2, 0, 4, "ababcd");
    test("dst-before-src", "abcdef", 0, 2, 4, "cdefef");
    test("same-ptr", "abcdef", 1, 1, 3, "abcdef");
    test("n=0", "abcdef", 0, 2, 0, "abcdef");

    char src[] = "tick";
    char dst[5] = "";
    myMemcpy(dst, src, 5); // 4 chars + '\0'
    check("memcpy", string(dst) == "tick");
}
