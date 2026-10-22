// LC 680 Valid Palindrome II: given a string, can it become a palindrome after deleting
// at most one character?
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

bool isPalindrome(const string& s, int lo, int hi) {
    while (lo < hi) {
        if (s[lo] != s[hi]) { return false; }
        lo++;
        hi--;
    }
    return true;
}

// Two pointers from both ends; at the first mismatch the one deletion must be either
// side, so check both remaining ranges. T: O(n), S: O(1).
bool validPalindrome(const string& s) {
    int lo = 0;
    int hi = (int)s.size() - 1;
    while (lo < hi) {
        if (s[lo] != s[hi]) {
            return isPalindrome(s, lo + 1, hi) || isPalindrome(s, lo, hi - 1);
        }
        lo++;
        hi--;
    }
    return true;
}

void test(const string& label, const string& s, bool want) {
    cout << label << (validPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("delete-right", "abbca", true);
    test("delete-left", "acbba", true);
    test("already", "aba", true);
    test("two-away", "abc", false);
}
