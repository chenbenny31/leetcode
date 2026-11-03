// ByteDance phone (FT), LC 5: return the longest palindromic substring of a non-empty
// lowercase string s; among equal lengths return the leftmost. The interviewer asked
// for O(n), i.e. Manacher's algorithm. (important/64 is the expand-only version.)
//   longestPalindrome(s) -> string
// Constraints: 1 <= n <= 2e5.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// Length of the palindrome grown from s[l..r] outward.
int expand(const string& s, int l, int r) {
    while (l >= 0 && r < (int)s.size() && s[l] == s[r]) {
        l--;
        r++;
    }
    return r - l - 1;
}

// Straightforward: expand around all 2n - 1 centers, left to right (odd center i, then
// the gap after i). A strictly longer palindrome replaces the best, so ties keep the
// leftmost start. T: O(n^2), S: O(1).
string longestPalindrome(const string& s) {
    int bestStart = 0;
    int bestLen = 1;
    for (int i = 0; i < (int)s.size(); i++) {
        int odd = expand(s, i, i);
        if (odd > bestLen) {
            bestLen = odd;
            bestStart = i - odd / 2;
        }
        int even = expand(s, i, i + 1);
        if (even > bestLen) {
            bestLen = even;
            bestStart = i - even / 2 + 1;
        }
    }
    return s.substr(bestStart, bestLen);
}

// Manacher: put '#' between characters ("aba" -> "#a#b#a#") so every palindrome has
// odd length in t. radius[i] = how far the palindrome centered at t[i] reaches.
// Keep the palindrome [center - right .. right] that reaches furthest right; for i
// inside it, the mirror 2 * center - i already knows a lower bound for radius[i], so
// expansion only does new work past `right`, and `right` only moves forward: O(n).
// In t, radius[i] equals the length of the original palindrome, which starts at
// (i - radius[i]) / 2. Strictly greater updates keep the leftmost among ties.
// T: O(n), S: O(n).
string longestPalindromeManacher(const string& s) {
    string t = "#";
    for (char c : s) {
        t += c;
        t += '#';
    }
    int m = t.size();
    // radius[i] = arm length of the longest palindrome in t centered at i
    vector<int> radius(m, 0);
    int center = 0;
    int right = 0;
    int bestCenter = 0;

    for (int i = 0; i < m; i++) {
        // reuse the mirror's answer while it stays inside the known palindrome
        if (i < right) {
            radius[i] = min(right - i, radius[2 * center - i]);
        }
        while (i - radius[i] - 1 >= 0 && i + radius[i] + 1 < m &&
               t[i - radius[i] - 1] == t[i + radius[i] + 1]) {
            radius[i]++;
        }

        if (i + radius[i] > right) {
            center = i;
            right = i + radius[i];
        }
        if (radius[i] > radius[bestCenter]) {
            bestCenter = i;
        }
    }

    int start = (bestCenter - radius[bestCenter]) / 2;
    return s.substr(start, radius[bestCenter]);
}

void test(const string& label, const string& s, const string& want) {
    bool ok = longestPalindrome(s) == want && longestPalindromeManacher(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", "z", "z");
    test("leftmost-tie", "abcd", "a");
    test("even", "xyyz", "yy");
    test("whole", "racecar", "racecar");
    test("nested", "abacabad", "abacaba");
}
