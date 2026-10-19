// lowering and dropping non-alphanumeric chars, return true if palindrome

#include <iostream>
#include <string>

using std::cout;
using std::string;

bool isAlnum(char c) {
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9');
}

char lower(char c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

// build clean string, comp by rev, T: O(n), S: O(n)
bool isPalindrome(const string& s) {
    string t;
    for (char c : s) {
        if (isAlnum(c)) { t += lower(c); }
    }
    return t == string(t.rbegin(), t.rend());
}

// two-pointers skip non-alnum chars, no copy, T: O(n), S: O(1)
bool isPalindromeInPlace(const string& s) {
    const int n = s.size();
    int left = 0;
    int right = n - 1;

    while (left < right) {
        if (!isAlnum(s[left])) { left++; continue; }
        if (!isAlnum(s[right])) { right--; continue; }
        if (lower(s[left]) != lower(s[right])) { return false; }
        left++;
        right--;
    }
    return true;
}

void test(const string& label, const string& s, bool want) {
    bool ok = isPalindrome(s) == want && isPalindromeInPlace(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", "A b,a", true);
    test("miss", "ab", false);
    test("digit", "0P", false);
    test("symbols", " .,", true);
}
