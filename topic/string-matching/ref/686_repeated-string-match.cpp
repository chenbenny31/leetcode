// LC 686 Repeated String Match: return the minimum number of times a must be repeated
// so that b is a substring of the result, or -1 if that is impossible.
#include <iostream>
#include <string>
using std::cout;
using std::string;

// Repeat a just enough to cover b, then one more copy for an unaligned start; any
// longer repeat adds nothing. T: O(|a| + |b|) with a linear-time search, S: O(|a| + |b|).
int repeatedStringMatch(string a, string b) {
    int times = 1;
    string s = a;   // a repeated `times` times
    while (s.size() < b.size()) {
        s += a;
        times++;
    }
    if (s.find(b) != string::npos) { return times; }
    s += a;
    if (s.find(b) != string::npos) { return times + 1; }
    return -1;
}

void test(const string& label, const string& a, const string& b, int want) {
    cout << label << (repeatedStringMatch(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("wrap-around", "abc", "cabcabca", 4);
    test("already-inside", "abcd", "bc", 1);
    test("absent-char", "abc", "abd", -1);
    test("exact", "ab", "abab", 2);
}
