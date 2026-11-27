// 24 · embedded basics list
// copy of `company/scientech/ref/recommended/21-memmove.cpp`
// memcpy / memmove — standard C++ hand-written task
// copy n bytes from src to dst and return dst
// (1) memcpy may assume the two ranges don't overlap  (2) memmove must work when they do
//
// clarify:
// signature -> same as the std ones: void* dst, const void* src, size_t n
// null ptrs, ranges shorter than n -> the caller's job, not checked
// speed -> byte by byte is fine (word at a time is follow-up 1)

#include <iostream>
#include <string>
#include <cstddef>

using std::cout;
using std::string;
using std::size_t;

// ==================== (1) ====================
// memcpy: the ranges never overlap

// core idea: void* has no size, so copy through byte ptrs, front to back
// T: O(n), S: O(1)
void* myMemcpy(void* dst, const void* src, size_t n) {
    unsigned char* to = (unsigned char*)dst;
    const unsigned char* from = (const unsigned char*)src;
    for (size_t i = 0; i < n; i++) { to[i] = from[i]; }
    return dst;
}

// ==================== (2) ====================
// memmove: the ranges may overlap (shifting data inside one buffer)

// core idea: pick the direction that never overwrites a byte it still has to read:
// dst before src -> forward, dst after src -> backward
// "abcdef", dst = src + 2, n = 4: forward gives ababab (wrong), backward gives ababcd
// T: O(n), S: O(1), no temp buffer
void* myMemmove(void* dst, const void* src, size_t n) {
    unsigned char* to = (unsigned char*)dst;
    const unsigned char* from = (const unsigned char*)src;
    if (to < from) {
        for (size_t i = 0; i < n; i++) { to[i] = from[i]; }
    } else if (to > from) {
        // i - 1 in the body: size_t can't go below 0, so `i >= 0` would never end
        for (size_t i = n; i > 0; i--) { to[i - 1] = from[i - 1]; }
    }
    return dst;
}

// ==================== tests ====================
void test(const string& label, string text, int dstPos, int srcPos, int n,
          const string& want) {
    void* ret = myMemmove(&text[dstPos], &text[srcPos], n);
    bool ok = text == want && ret == &text[dstPos];
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-overlap", "abcdef", 0, 3, 3, "defdef");
    test("dst-after-src", "abcdef", 2, 0, 4, "ababcd");
    test("dst-before-src", "abcdef", 0, 2, 4, "cdefef");
    test("n=0", "abcdef", 0, 2, 0, "abcdef");

    char src[] = "tick";
    char dst[5] = "";
    myMemcpy(dst, src, 5);
    check(string(dst) == "tick", "memcpy");
}

// follow-up:
// 1. faster -> copy 8 bytes at a time once both ptrs are aligned, bytes for the tail
// 2. memcpy on overlapping ranges -> undefined behavior; that is what memmove is for
// 3. why unsigned char* -> it may alias any object, and void* has no arithmetic
// 4. objects with ctors -> never memcpy them; only trivially copyable types
// 5. strcpy / strlen -> the same byte loop, stopping at '\0' instead of at n
// 6. memset -> the same loop writing one byte value
