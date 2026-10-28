// packages is a string of 'A'-'Z' of length n. A prefix of length L is valid if L divides
// n and every length-L block of the string has exactly the prefix's letter counts.
// Return the number of valid prefixes.

#include <iostream>
#include <vector>
#include <string>
#include <array>

using std::cout;
using std::string;
using std::vector;
using std::array;

// straightforward: for each divisor L, count each block's letters directly.
// T: O(n * d(n)) for d(n) divisors, S: O(1).
int validPrefixes(const string& s) {
    int n = s.size(), valid = 0;
    for (int L = 1; L <= n; L++) {
        if (n % L != 0) { continue; }
        int want[26] = {};
        for (int i = 0; i < L; i++) { want[s[i] - 'A']++; }
        bool ok = true;
        for (int start = L; start < n && ok; start += L) {
            int have[26] = {};
            for (int i = start; i < start + L; i++) { have[s[i] - 'A']++; }
            for (int c = 0; c < 26; c++) {
                if (have[c] != want[c]) { ok = false; }
            }
        }
        if (ok) { valid++; }
    }
    return valid;
}

// improved: prefix counts give any block's counts in O(26), so each divisor costs
// O(26 * n / L). T: O(26 * n log n) worst case, S: O(26 * n).
int validPrefixesCounts(const string& s) {
    int n = s.size();
    // pre[i][c] = occurrences of letter c in s[0..i)
    vector<array<int, 26>> pre(n + 1);
    pre[0].fill(0);
    for (int i = 0; i < n; i++) {
        pre[i + 1] = pre[i];
        pre[i + 1][s[i] - 'A']++;
    }
    int valid = 0;
    for (int L = 1; L <= n; L++) {
        if (n % L != 0) { continue; }
        bool ok = true;
        for (int start = L; start < n && ok; start += L) {
            for (int c = 0; c < 26; c++) {
                if (pre[start + L][c] - pre[start][c] != pre[L][c]) { ok = false; }
            }
        }
        if (ok) { valid++; }
    }
    return valid;
}

void test(const string& label, const string& s, int want) {
    bool ok = validPrefixes(s) == want && validPrefixesCounts(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat", "ABAB", 2);
    test("only-whole", "AABB", 1);
    test("same-letter", "AAAA", 3);
    test("shuffled-block", "ABBA", 2);
    test("single", "Z", 1);
}
