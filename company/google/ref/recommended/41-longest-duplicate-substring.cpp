// Longest Duplicate Substring (LC 1044, also Google interview report). s is lowercase.
// Return a longest substring that occurs at least twice (occurrences may overlap), or ""
// if none. Ties: any one of the longest (tests avoid ties).

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;

// straightforward: lengths from long to short; the first length whose substrings repeat
// wins. T: O(n^3) (n lengths x n substrings x O(n) hashing), S: O(n^2).
string longestDupSubstringNaive(const string& s) {
    int n = s.size();
    for (int len = n - 1; len >= 1; len--) {
        // substrings of length len seen so far
        unordered_set<string> seen;
        for (int start = 0; start + len <= n; start++) {
            string sub = s.substr(start, len);
            if (!seen.insert(sub).second) { return sub; }
        }
    }
    return "";
}

// Rabin-Karp: rolling hash of every length-len window, mod 2^64 via unsigned overflow.
// Equal hashes are verified with compare, so a collision costs time, never correctness.
// Returns the start of a repeated length-len substring, or -1. T: O(n) expected, S: O(n).
int duplicateStart(const string& s, int len) {
    const unsigned long long base = 131;
    // power = base^len, the weight of the char leaving the window
    unsigned long long power = 1;
    for (int i = 0; i < len; i++) { power *= base; }
    // starts[h] = window starts whose hash is h
    unordered_map<unsigned long long, vector<int>> starts;
    unsigned long long hash = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        hash = hash * base + s[i];
        if (i >= len) { hash -= s[i - len] * power; }
        if (i < len - 1) { continue; }
        int start = i - len + 1;
        for (int other : starts[hash]) {
            if (s.compare(other, len, s, start, len) == 0) { return other; }
        }
        starts[hash].push_back(start);
    }
    return -1;
}

// improved: a repeat of length L contains a repeat of length L - 1, so binary search the
// largest len with duplicateStart != -1. T: O(n log n) expected, S: O(n).
string longestDupSubstring(const string& s) {
    int lo = 1;
    int hi = (int)s.size() - 1;
    int bestStart = -1;
    int bestLen = 0;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int start = duplicateStart(s, mid);
        if (start == -1) {
            hi = mid - 1;
            continue;
        }
        bestStart = start;
        bestLen = mid;
        lo = mid + 1;
    }
    return bestLen == 0 ? "" : s.substr(bestStart, bestLen);
}

void test(const string& label, const string& s, const string& want) {
    bool ok = longestDupSubstringNaive(s) == want && longestDupSubstring(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat", "abcxabc", "abc");
    test("overlap", "aaaa", "aaa");
    test("longer-wins", "abcdbcdab", "bcd");
    test("no-dup", "abc", "");
    test("single", "a", "");
}

// follow-up: why verify? mod 2^64 is fast but has known adversarial collisions
//   (Thue-Morse strings); without compare, a collision returns a wrong answer. A large
//   prime mod (1e9 + 7, long long) is harder to break but has ~n^2 / p birthday
//   collisions; double hashing (two mods) makes them negligible.
// follow-up: guaranteed O(n log n) / O(n) -> suffix array + LCP array: the answer is the
//   max adjacent LCP; or a suffix automaton
