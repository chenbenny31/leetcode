// LC 125 Valid Palindrome (Easy) — two pointers from both ends
// after lowercasing and dropping non-alphanumeric chars, is s a palindrome?
//
// clarify:
// digits -> count as chars
// empty after filtering -> palindrome
// charset -> ASCII only
// extra space -> O(1) wanted, so no filtered copy

#include <iostream>
#include <string>
#include <cctype>

using std::cout;
using std::string;
using std::isalnum;
using std::tolower;

// core idea: move both pointers inward, skip non-alnum, compare the lowercased pair
// T: O(n), S: O(1)
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

// follow-up:
// delete at most one char (LC 680) -> at first mismatch check s[l+1..r] or s[l..r-1]
// at most k deletions (LC 1216) -> n - longest palindromic subsequence <= k; O(n^2) DP
