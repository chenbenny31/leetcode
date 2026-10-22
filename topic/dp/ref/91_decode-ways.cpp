// LC 91 Decode Ways: 'A'->"1" ... 'Z'->"26". Given a digit string s, count the ways
// to decode it (a leading zero cannot be decoded).
#include <iostream>
#include <string>
using std::cout;
using std::string;

// ways(i) = one-digit decode of s[i-1] + two-digit decode of s[i-2..i), O(1) state.
// T: O(n), S: O(1).
int numDecodings(const string& s) {
    int n = s.size();
    // two, one = ways to decode the prefixes of length i-2 and i-1
    int two = 1, one = s[0] != '0' ? 1 : 0;
    for (int i = 2; i <= n; i++) {
        int cur = 0;
        if (s[i - 1] != '0') {
            cur += one;
        }
        int num = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');
        if (num >= 10 && num <= 26) {
            cur += two;
        }
        two = one;
        one = cur;
    }
    return one;
}

void test(const string& label, const string& s, int want) {
    cout << label << (numDecodings(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("ones", "111", 3);
    test("too-big", "27", 1);
    test("zero-pair", "100", 0);
    test("leading-zero", "06", 0);
}
