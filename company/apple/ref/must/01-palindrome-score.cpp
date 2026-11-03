// Apple: score 0..100 for how closely a string matches a palindrome. Strict policy:
// compare each normalized mirrored pair equally; a pair scores 1 only if both chars
// match. score = 100 * matched / pairs; empty or one-char input = 100.
// Follow-up: explicit, versioned opt-in policy (fuzzy similarity + positional weights).
// Note: for user-facing Unicode, compare grapheme clusters (e.g. ICU) instead of chars.
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_map>
using std::cout;
using std::string;
using std::fabs;
using std::unordered_map;

// normalization: ASCII lowercase, so 'R' and 'r' match
char lower(char c) { return c >= 'A' && c <= 'Z' ? c - 'A' + 'a' : c; }

// two pointers over mirrored pairs; the middle char of an odd length has no pair.
// T: O(n), S: O(1).
double palindromeScore(const string& s) {
    int n = s.size();
    int pairs = n / 2, matched = 0;
    for (int l = 0, r = n - 1; l < r; l++, r--) {
        if (lower(s[l]) == lower(s[r])) { matched++; }
    }
    // no pair to compare -> 100 by definition (also avoids dividing by 0)
    return pairs == 0 ? 100.0 : 100.0 * matched / pairs;
}

// follow-up: an opt-in policy is plain data, so every score names how it was computed
struct Policy {
    string version;      // e.g. "lookalike-outer-v1", stored next to each score
    double lookalike;    // credit for a lookalike pair, e.g. 0.5 for ('0', 'o')
    bool outerHeavier;   // pair k (0 = outermost) weighs pairs - k; otherwise all 1
};

// per pair: identical = 1, lookalike = p.lookalike, else 0;
// score = 100 * sum(w * sim) / sum(w), still in 0..100. T: O(n), S: O(1).
double palindromeScore(const string& s, const Policy& p) {
    // defined lookalike mapping (symmetric), applied after lowercasing
    unordered_map<char, char> like = {{'0', 'o'}, {'o', '0'}, {'1', 'l'}, {'l', '1'}};
    int n = s.size();
    int pairs = n / 2;
    if (pairs == 0) { return 100.0; }
    double got = 0, total = 0;
    for (int k = 0; k < pairs; k++) {
        char a = lower(s[k]), b = lower(s[n - 1 - k]);
        double sim = 0;
        if (a == b) {
            sim = 1;
        } else if (like.count(a) && like[a] == b) {
            sim = p.lookalike;
        }
        double w = p.outerHeavier ? pairs - k : 1;
        got += w * sim;
        total += w;
    }
    return 100.0 * got / total;
}

void test(const string& label, const string& s, double want) {
    cout << label << (fabs(palindromeScore(s) - want) < 1e-9 ? " PASS\n" : " FAIL\n");
}

int main() {
    test("abcca", "abcca", 50.0);
    test("racecar", "Racecar", 100.0);
    test("abcd", "abcd", 0.0);
    test("empty", "", 100.0);
}
