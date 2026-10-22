// LC 5 Longest Palindromic Substring: given a string s, return its longest
// palindromic substring.
#include <iostream>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::string;

// Length of the palindrome grown outward from the centre (l, r). T: O(n), S: O(1).
int expand(const string& s, int l, int r) {
    int n = s.size();
    while (l >= 0 && r < n && s[l] == s[r]) {
        l--;
        r++;
    }
    return r - l - 1;
}

// Expand around each of the 2n-1 centres, keep the longest. T: O(n^2), S: O(1).
string longestPalindrome(const string& s) {
    int n = s.size();
    int start = 0, best = 0;   // best palindrome so far is s[start, start + best)
    for (int i = 0; i < n; i++) {
        int len = max(expand(s, i, i), expand(s, i, i + 1));
        if (len > best) {
            best = len;
            start = i - (len - 1) / 2;
        }
    }
    return s.substr(start, best);
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (longestPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd", "xabay", "aba");
    test("even", "cbbd", "bb");
    test("single", "z", "z");
    test("all-same", "aaaa", "aaaa");
}
