// Longest Duplicate Substring (LC 1044, also Google interview report). s is lowercase.
// return a longest substring that occurs at least twice (overlap ok), "" if none
//
// clarify:
// n up to ~3e4 -> need O(n log n)
// overlapping occurrences -> they cnt
// ties -> any one of the longest
// substring or its len -> substring
// no repeat or n <= 1 -> ""

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

// solution 1: brute force; baseline
// core idea: try lens long to short w/ a hash set of substrings; first repeat wins
// T: O(n^3), S: O(n^2)
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

// start of a repeated length-len substring, or -1: rolling hash, equal hashes verified
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

// solution 2: binary search + Rabin-Karp; the expected answer
// core idea: a repeat of len L contains a repeat of len L - 1 -> binary search max len
// one len is checked in O(n) by rolling hash (mod 2^64 via unsigned wrap-around)
// T: O(n log n), S: O(n) — expected; equal hashes are verified
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

// solution 3: suffix array + Kasai LCP; no hashing, guaranteed bound
// core idea: equal substrings are prefixes of two suffixes -> answer = max neighbor LCP
// T: O(n log^2 n), S: O(n) — prefix doubling: log n rounds of sorting
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
    test("overlap", "aaaa", "aaa");
    test("longer-wins", "abcdbcdab", "bcd");
    test("no-dup", "abc", "");
    test("empty", "", "");
}

// follow-up:
// why compare on equal hashes -> mod 2^64 has known adversarial collisions (Thue-Morse)
// large prime mod / double hashing -> only makes collisions unlikely
// faster suffix array -> radix sort makes it O(n log n); suffix automaton is O(n)
// repeated at least k times -> same binary search; succeed once a window has k copies
