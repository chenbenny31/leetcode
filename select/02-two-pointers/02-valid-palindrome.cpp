// LC 125 Valid Palindrome (Easy) — two pointers from both ends
// After lowercasing and dropping every non-alphanumeric char, is s a palindrome?
// Clarify: digits count as characters; an empty result is a palindrome; ASCII only;
//   O(1) extra space wanted.

#include <iostream>
#include <string>
#include <cctype>

using std::cout;
using std::string;
using std::isalnum;
using std::tolower;

// Core idea: move both pointers inward, skipping non-alphanumeric chars, and compare the
// lowercased pair. T: O(n), S: O(1).
bool isPalindrome(const string& s) {
    int left = 0;
    int right = (int)s.size() - 1;
    while (left < right) {
        if (!isalnum(s[left])) {
            left++;
        } else if (!isalnum(s[right])) {
            right--;
        } else if (tolower(s[left]) != tolower(s[right])) {
            return false;
        } else {
            left++;
            right--;
        }
    }
    return true;
}

void test(const string& label, const string& s, bool want) {
    cout << label << (isPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("case-and-symbols", "No, on!", true);
    test("mismatch", "abca", false);
    test("digit-vs-letter", "2R", false);
    test("only-symbols", " ,.", true);
    return 0;
}

// follow-up (ideas only):
// - palindrome after deleting at most one char (LC 680) -> at the first mismatch, check
//   s[left + 1..right] or s[left..right - 1] with a plain range check; O(n)
// - at most k deletions (LC 1216) -> n - longest palindromic subsequence <= k,
//   an O(n^2) DP
