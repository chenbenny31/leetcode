// LC 28 Find First Occurrence in a String (Easy) — string matching: rolling hash / KMP
// Return the index of the first occurrence of needle in haystack, or -1 if absent.
// Clarify: empty needle (return 0); needle longer than haystack; alphabet; whether
//   the O(n * m) scan is enough or a linear algorithm is wanted.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: compare the hash of each length-m window with the needle's hash; sliding
// the window updates the hash in O(1), and a hash hit is verified with a real compare.
// T: O(n + m) expected, O(n * m) worst, S: O(1).
int strStrRabinKarp(const string& haystack, const string& needle) {
    int n = haystack.size();
    int m = needle.size();
    if (m > n) { return -1; }
    // hashes are polynomials in base, taken mod 2^64 by unsigned overflow
    const unsigned long long base = 131;
    // base^(m - 1), the weight of the char that leaves the window
    unsigned long long top = 1;
    for (int i = 1; i < m; i++) { top *= base; }
    unsigned long long want = 0;
    unsigned long long window = 0;
    for (int i = 0; i < m; i++) {
        want = want * base + (unsigned char)needle[i];
        window = window * base + (unsigned char)haystack[i];
    }
    for (int start = 0; start + m <= n; start++) {
        if (window == want && haystack.compare(start, m, needle) == 0) { return start; }
        if (start + m < n) {
            window -= (unsigned char)haystack[start] * top;
            window = window * base + (unsigned char)haystack[start + m];
        }
    }
    return -1;
}

// Core idea: KMP: on a mismatch after `matched` chars, fall back to the longest proper
// prefix of the needle that is also a suffix of the matched part; haystack never rewinds.
// T: O(n + m), S: O(m).
int strStrKmp(const string& haystack, const string& needle) {
    int n = haystack.size();
    int m = needle.size();
    if (m == 0) { return 0; }
    // fail[i] = length of the longest proper prefix of needle[0..i] that is also
    // its suffix
    vector<int> fail(m, 0);
    int matched = 0;
    for (int i = 1; i < m; i++) {
        while (matched > 0 && needle[i] != needle[matched]) {
            matched = fail[matched - 1];
        }
        if (needle[i] == needle[matched]) { matched++; }
        fail[i] = matched;
    }
    // matched = needle chars matched so far, ending at haystack[i]
    matched = 0;
    for (int i = 0; i < n; i++) {
        while (matched > 0 && haystack[i] != needle[matched]) {
            matched = fail[matched - 1];
        }
        if (haystack[i] == needle[matched]) { matched++; }
        if (matched == m) { return i - m + 1; }
    }
    return -1;
}

void test(const string& label, const string& haystack, const string& needle, int want) {
    bool ok = strStrRabinKarp(haystack, needle) == want &&
              strStrKmp(haystack, needle) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("first-of-two", "abab", "ab", 0);
    test("overlap-fallback", "aaab", "aab", 1);
    test("match-at-end", "bananas", "nas", 4);
    test("needle-longer", "ab", "abc", -1);
    return 0;
}

// follow-up (ideas only):
// - repeated substring pattern (LC 459) -> s occurs inside (s + s) with the first and
//   last char removed; or with the KMP table: len % (len - fail[len - 1]) == 0
// - shortest palindrome (LC 214) -> KMP table of s + '#' + reverse(s); its last value is
//   the longest palindromic prefix, prepend the reverse of the rest
// - longest duplicate substring (LC 1044) -> binary search on the length; for a length,
//   roll a hash over all windows and look for a repeat in a set
// - many patterns at once -> trie of the patterns, or Aho-Corasick (trie + failure
//   links, the multi-pattern KMP)
// - hash collisions -> verify each hit with a compare (as here), or use two independent
//   hashes (double hashing) when a verify is too slow
