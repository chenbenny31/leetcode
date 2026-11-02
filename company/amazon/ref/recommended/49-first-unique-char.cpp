// LC 387: return the index of the first character of s (lowercase) that appears exactly
// once, or -1.

#include <iostream>
#include <string>

using std::cout;
using std::string;

// count, then the first index with count 1. T: O(n), S: O(26).
int firstUniqChar(const string& s) {
    // cnt[c] = occurrences of letter c
    int cnt[26] = {};
    for (char c : s) { cnt[c - 'a']++; }
    int n = s.size();
    for (int i = 0; i < n; i++) {
        if (cnt[s[i] - 'a'] == 1) { return i; }
    }
    return -1;
}

void test(const string& label, const string& s, int want) {
    cout << label << (firstUniqChar(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("first", "abca", 1);
    test("last", "aab", 2);
    test("none", "aa", -1);
    test("single", "z", 0);
}
