// LC 5: return the longest palindromic substring of s.
#include <iostream>
#include <string>
using std::cout;
using std::string;

// grow s[l..r] outward; record it if it beats the best so far
void expand(const string& s, int l, int r, int& start, int& best) {
    int n = s.size();
    while (l >= 0 && r < n && s[l] == s[r]) {
        l--;
        r++;
    }
    // loop overshoots by one on each side: palindrome is s[l+1 .. r-1]
    if (r - l - 1 > best) {
        best = r - l - 1;
        start = l + 1;
    }
}

// Expand around every center: a char (odd) and a gap (even). T: O(n^2), S: O(1).
string longestPalindrome(const string& s) {
    int start = 0, best = 0;   // best palindrome = s.substr(start, best)
    int n = s.size();
    for (int i = 0; i < n; i++) {
        expand(s, i, i, start, best);
        expand(s, i, i + 1, start, best);
    }
    return s.substr(start, best);
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (longestPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd", "cabad", "aba");
    test("even", "cbbd", "bb");
    test("single", "a", "a");
}
