// s can be rearranged into a palindrome. Return the lexicographically smallest
// palindrome made from all of its characters.

#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::reverse;

// Put the smallest letters as early as possible: the left half is every letter's count
// / 2 in a..z order, the odd letter (if any) goes in the middle. T: O(n), S: O(n).
string smallestPalindrome(const string& s) {
    // cnt[c] = occurrences of letter c
    int cnt[26] = {};
    for (char c : s) { cnt[c - 'a']++; }
    string half, mid;
    for (int c = 0; c < 26; c++) {
        half += string(cnt[c] / 2, 'a' + c);
        if (cnt[c] % 2 == 1) { mid = string(1, 'a' + c); }
    }
    string right = half;
    reverse(right.begin(), right.end());
    return half + mid + right;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (smallestPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd-mid", "cabba", "abcba");
    test("even", "bbaa", "abba");
    test("big-mid", "babab", "abbba");
    test("single", "z", "z");
}
