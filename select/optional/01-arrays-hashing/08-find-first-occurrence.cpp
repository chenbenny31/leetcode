// LC 28 Find First Occurrence in a String (Easy) — string matching: rolling hash / KMP
// return idx of first occurrence of needle in haystack, or -1
//
// clarify:
// empty needle -> return 0
// needle longer than haystack -> -1
// alphabet -> any bytes, hash on unsigned char
// O(n * m) scan enough? -> if not: rolling hash or KMP

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// solution 1: Rabin-Karp (rolling hash); easy to extend to many windows / lengths
// core idea: hash each len-m window, slide in O(1); verify a hash hit w/ real compare
// T: O(n + m), S: O(1) — expected; O(n * m) worst w/ collisions
int strStrRabinKarp(const string& haystack, const string& needle) {
    int n = haystack.size();
    int m = needle.size();
    if (m > n) { return -1; }
    // hash = polynomial in base, mod 2^64 by unsigned overflow
    const unsigned long long base = 131;
    // top = base^(m - 1), weight of the char leaving the window
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

// solution 2: KMP; use it when a guaranteed linear bound is asked
// core idea: on mismatch fall back to longest prefix = suffix of the matched part
// haystack ptr never rewinds
// T: O(n + m), S: O(m) — matched only drops as much as it grew
int strStrKmp(const string& haystack, const string& needle) {
    int n = haystack.size();
    int m = needle.size();
    if (m == 0) { return 0; }
    // fail[i] = len of longest proper prefix of needle[0..i] that is also its suffix
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

// follow-up:
// repeated substring pattern (LC 459) -> s inside (s + s) w/o first and last char
// same w/ KMP table -> len % (len - fail[len - 1]) == 0
// shortest palindrome (LC 214) -> fail of s + '#' + rev(s); last = palindromic prefix len
// longest dup substring (LC 1044) -> binary search on len; roll hash, repeat in a set
// many patterns at once -> trie of patterns, or Aho-Corasick (trie + failure links)
// hash collisions -> verify each hit (as here), or double hashing if verify is too slow
