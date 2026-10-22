// LC 459 Repeated Substring Pattern: return true if s can be built by repeating one of
// its substrings two or more times.
#include <iostream>
#include <string>
using std::cout;
using std::string;

// Rotation trick: s repeats a block iff s occurs inside s + s at some shift 1..n-1.
// With a linear-time search (KMP) T: O(n), S: O(n).
bool repeatedSubstringPattern(string s) {
    int n = s.size();
    return (s + s).find(s, 1) != (size_t)n;
}

void test(const string& label, const string& s, bool want) {
    cout << label << (repeatedSubstringPattern(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeats", "abcabcabc", true);
    test("no-repeat", "abcab", false);
    test("single-char", "a", false);
    test("same-chars", "zzzz", true);
}
