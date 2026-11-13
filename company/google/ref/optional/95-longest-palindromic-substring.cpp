// 95 · Longest Palindromic Substring — LC 5
// Mother: select/optional/13-dp/09-longest-palindromic-substring.cpp
// longest substring of s that reads the same both ways; any one if tied
// reported: LC tag #7
//
// clarify:
// ties -> any one
// empty string -> ""
// case sensitive? -> assume yes
// n large? -> O(n^2) ok for n ~ 1e3; else Manacher (follow-up)

#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::max;

// grow s[left..right] outward while palindrome; return final len
int expand(const string& s, int left, int right) {
    int n = s.size();
    while (left >= 0 && right < n && s[left] == s[right]) {
        left--;
        right++;
    }
    // palindrome = s[left + 1..right - 1]
    return right - left - 1;
}

// core idea: try all 2n - 1 centers (each char = odd, each gap = even), expand outward
// T: O(n^2), S: O(1)
string longestPalindrome(const string& s) {
    // best palindrome = s.substr(start, best)
    int start = 0;
    int best = 0;
    int n = s.size();
    for (int i = 0; i < n; i++) {
        int len = max(expand(s, i, i), expand(s, i, i + 1));
        if (len > best) {
            best = len;
            // works for both: odd centers on i, even on gap after i
            start = i - (len - 1) / 2;
        }
    }
    return s.substr(start, best);
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (longestPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd", "cabad", "aba");
    test("even", "xabbay", "abba");
    test("all-same", "aaaa", "aaaa");
    test("single", "z", "z");
}

// follow-up:
// count palindromic substrings (LC 647) -> same 2n - 1 centers, +1 per expand step
// O(n) -> Manacher: reuse mirror's radius inside rightmost palindrome
