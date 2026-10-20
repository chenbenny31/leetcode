// return the longest palindromic substring of s

#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::max;

void expand(const string& s, int l, int r, int& start, int& maxLen) {
    int n = s.size();
    while (l >= 0 && r < n && s[l] == s[r]) {
        l--;
        r++;
    }
    if (r - l - 1 > maxLen) {
        maxLen = r - l - 1;
        start = l + 1;
    }
}

// expand around every center, div into odd/even, T: O(n^2), S: O(1)
string longestPalindrome(const string& s) {
    int start = 0;
    int maxLen = 0; // max Palindrome = s.substr(start, maxLen)
    int n = s.size();
    for (int i = 0; i < n; i++) {
        expand(s, i, i, start, maxLen);
        expand(s, i, i + 1, start, maxLen);
    }
    return s.substr(start, maxLen);
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (longestPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd", "cabad", "aba");
    test("even", "cbbd", "bb");
    test("single", "a", "a");
}
