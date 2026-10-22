// LC 28 Find the Index of the First Occurrence in a String: return the first index where
// needle occurs in haystack, or -1 if it does not occur.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// KMP: on a mismatch fall back along the failure function; the haystack index never
// moves back. T: O(n + m), S: O(m).
int strStr(string haystack, string needle) {
    int n = haystack.size(), m = needle.size();
    // lps[i] = length of the longest proper prefix of needle[0..i] that is also a suffix
    vector<int> lps(m, 0);
    int len = 0;             // length of the border currently being extended
    for (int i = 1; i < m; i++) {
        while (len > 0 && needle[i] != needle[len]) { len = lps[len - 1]; }
        if (needle[i] == needle[len]) { len++; }
        lps[i] = len;
    }
    int j = 0;               // chars of needle matched so far
    for (int i = 0; i < n; i++) {
        while (j > 0 && haystack[i] != needle[j]) { j = lps[j - 1]; }
        if (haystack[i] == needle[j]) { j++; }
        if (j == m) { return i - m + 1; }
    }
    return -1;
}

void test(const string& label, const string& haystack, const string& needle, int want) {
    cout << label << (strStr(haystack, needle) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", "hello world", "o w", 4);
    test("fallback", "aabaaabaaac", "aabaaac", 4);
    test("absent", "abc", "abd", -1);
    test("longer-needle", "ab", "abc", -1);
}
