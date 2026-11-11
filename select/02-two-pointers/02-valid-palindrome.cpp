// LC 125 Valid Palindrome (Easy). Pattern: two pointers from both ends.
// After lowercasing and dropping every non-alphanumeric char, is s a palindrome?
// An empty result counts as a palindrome.

#include <iostream>
#include <string>
#include <cctype>

using std::cout;
using std::string;
using std::isalnum;
using std::tolower;

// Move inward, skipping non-alnum chars; compare lowercased. T: O(n), S: O(1).
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

// Is s[left..right] a palindrome (exact chars)? T: O(right - left), S: O(1).
bool isRangePalindrome(const string& s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

// LC 680, plain lowercase input (no alnum filtering): at the first mismatch, delete
// s[left] or s[right] and check the rest once more. T: O(n), S: O(1).
bool validPalindromeII(const string& s) {
    int left = 0;
    int right = (int)s.size() - 1;
    while (left < right) {
        if (s[left] != s[right]) {
            // the one deletion is spent here; the rest must match exactly
            return isRangePalindrome(s, left + 1, right)
                   || isRangePalindrome(s, left, right - 1);
        }
        left++;
        right--;
    }
    return true;
}

void test(const string& label, const string& s, bool want) {
    cout << label << (isPalindrome(s) == want ? " PASS\n" : " FAIL\n");
}

void testDeleteOne(const string& label, const string& s, bool want) {
    cout << label << (validPalindromeII(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("case-and-symbols", "No, on!", true);
    test("mismatch", "abca", false);
    test("digit-vs-letter", "2R", false);
    test("only-symbols", " ,.", true);
    testDeleteOne("delete-left", "xaba", true);
    testDeleteOne("delete-right", "abax", true);
    testDeleteOne("needs-two", "abxyzba", false);
    testDeleteOne("no-delete", "abba", true);
}
