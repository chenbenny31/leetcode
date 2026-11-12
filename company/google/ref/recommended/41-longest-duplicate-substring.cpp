// Longest Duplicate Substring (LC 1044, also Google interview report). s is lowercase.
// Return a longest substring that occurs at least twice (occurrences may overlap), or ""
// if none. Ties: any one of the longest (tests avoid ties).
//
// Clarify:
// - n up to ~3e4 (LC 1044) -> n^2 substrings is too many; O(n log n) expected
// - lowercase only (stated) -> base 131 is above every char value
// - overlapping occurrences count ("aaaa" -> "aaa")
// - none -> ""; n = 0 or 1 -> ""
// - return the substring, not its length -> keep the start of a match
// Idea: a repeat of length L contains a repeat of length L - 1 (drop its last char), so
//   "some length-len substring repeats" is true...true false...false: binary search the
//   last true len. Check one len in O(n) with a rolling hash over every window
//   (Rabin-Karp); windows with equal hashes are compared before trusting them.
// Pitfalls:
// - binary search for the LAST true: lo = 1, hi = n - 1, while (lo <= hi); on success
//   save the start and lo = mid + 1, else hi = mid - 1. Save at success time: the final
//   mid may be a failure
// - hi = n - 1, not n (a length-n substring cannot repeat); n = 0 or 1 never indexes s
// - rolling hash: add s[i] first, subtract s[i - len] * base^len only when i >= len; the
//   window is complete only from i = len - 1
// - unsigned long long: wrap-around is defined; signed overflow is UB
// - keep every start per hash, not just the first: after a collision a later window may
//   equal the second string; and always compare, mod 2^64 has known collisions
// - suffix array: pos[i] == 0 has no previous suffix; check it before sa[pos[i] - 1]
// Tests to write: repeat far apart; overlapping repeat; a longer repeat beats a shorter
//   one; no repeat -> ""; one char; empty.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::pair;
using std::sort;

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

// follow-up, no hashing: suffix array by prefix doubling (each round sorts suffixes by
// the ranks of their first k and next k chars), then Kasai's LCP with the previous
// suffix in sorted order. Two equal substrings are prefixes of two suffixes, so the
// answer is the largest LCP of neighbors. T: O(n log^2 n), S: O(n).
string longestDupSuffixArray(const string& s) {
    int n = s.size();
    // sa = suffix starts in sorted order
    vector<int> sa(n);
    // rank[i] = class of suffix i by its first k chars (equal prefixes, equal rank)
    vector<int> rank(n);
    for (int i = 0; i < n; i++) {
        sa[i] = i;
        rank[i] = s[i];
    }
    for (int k = 1; k < n; k *= 2) {
        // key[i] = (rank of the first k chars, rank of the next k or -1 past the end)
        vector<pair<int, int>> key(n);
        for (int i = 0; i < n; i++) { key[i] = {rank[i], i + k < n ? rank[i + k] : -1}; }
        sort(sa.begin(), sa.end(), [&](int a, int b) { return key[a] < key[b]; });
        rank[sa[0]] = 0;
        for (int i = 1; i < n; i++) {
            bool same = key[sa[i]] == key[sa[i - 1]];
            rank[sa[i]] = rank[sa[i - 1]] + (same ? 0 : 1);
        }
    }
    // pos[i] = place of suffix i in sa
    vector<int> pos(n);
    for (int i = 0; i < n; i++) { pos[sa[i]] = i; }
    int bestStart = 0;
    int bestLen = 0;
    // lcp of suffix i and the one before it; drops by at most 1 when i moves right
    int lcp = 0;
    for (int i = 0; i < n; i++) {
        // pos[i] == 0: no suffix before it
        if (pos[i] == 0) {
            lcp = 0;
            continue;
        }
        int j = sa[pos[i] - 1];
        while (i + lcp < n && j + lcp < n && s[i + lcp] == s[j + lcp]) { lcp++; }
        if (lcp > bestLen) {
            bestLen = lcp;
            bestStart = i;
        }
        if (lcp > 0) { lcp--; }
    }
    return s.substr(bestStart, bestLen);
}

void test(const string& label, const string& s, const string& want) {
    bool ok = longestDupSubstringNaive(s) == want && longestDupSubstring(s) == want &&
              longestDupSuffixArray(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat", "abcxabc", "abc");
    test("overlap", "aaaa", "aaa");
    test("longer-wins", "abcdbcdab", "bcd");
    test("no-dup", "abc", "");
    test("single", "a", "");
    test("empty", "", "");
}

// follow-up: why verify? mod 2^64 is fast but has known adversarial collisions
//   (Thue-Morse strings); without compare, a collision returns a wrong answer. A large
//   prime mod (1e9 + 7, long long) is harder to break but has ~n^2 / p birthday
//   collisions; double hashing (two mods) makes them negligible.
// follow-up: guaranteed O(n log n) / O(n), no hashing -> longestDupSuffixArray above
//   (O(n log^2 n) with sort; radix sort makes it O(n log n), a suffix automaton O(n))
// follow-up: longest substring repeated at least k times -> same binary search; count the
//   verified copies of each window and succeed once one reaches k; with the suffix
//   array, the max over every k consecutive suffixes of the min of their k - 1 LCPs
//   (sliding-window minimum)
