// LC 1044 Longest Duplicate Substring: return any longest substring of lowercase s that
// occurs at least twice (occurrences may overlap), or "" if there is none.
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

const long long MOD = (1LL << 61) - 1;   // large prime keeps collisions negligible
const long long BASE = 131;

long long mulmod(long long a, long long b) {
    return (__int128)a * b % MOD;
}

// Rabin-Karp: rolling hash of every window of `len`; the first repeated hash is a
// duplicate. Returns its start, or -1. T: O(n), S: O(n).
int dupStart(const string& s, int len) {
    int n = s.size();
    long long hash = 0;   // hash of the current window
    long long top = 1;    // BASE^len, the weight of the char leaving the window
    for (int i = 0; i < len; i++) {
        hash = (mulmod(hash, BASE) + (s[i] - 'a' + 1)) % MOD;
        top = mulmod(top, BASE);
    }
    unordered_map<long long, int> seen;   // window hash -> start of its first occurrence
    seen[hash] = 0;
    for (int i = len; i < n; i++) {
        hash = (mulmod(hash, BASE) + (s[i] - 'a' + 1)) % MOD;
        hash = (hash + MOD - mulmod(s[i - len] - 'a' + 1, top)) % MOD;
        int start = i - len + 1;
        if (seen.count(hash)) { return start; }
        seen[hash] = start;
    }
    return -1;
}

// Binary search on the length: a duplicate of length L implies one of length L-1.
// T: O(n log n), S: O(n).
string longestDupSubstring(string s) {
    int lo = 1, hi = (int)s.size() - 1;
    string best;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        int start = dupStart(s, mid);
        if (start == -1) {
            hi = mid - 1;
        } else {
            best = s.substr(start, mid);
            lo = mid + 1;
        }
    }
    return best;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (longestDupSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("inner-repeat", "xabcyabc", "abc");
    test("all-distinct", "abcd", "");
    test("overlap", "aaaa", "aaa");
    test("single-char", "a", "");
}
