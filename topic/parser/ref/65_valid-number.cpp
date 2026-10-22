// LC 65 Valid Number: decide whether s is a valid number: an optional sign, digits with
// an optional '.', and an optional exponent 'e'/'E' with a signed integer after it.
#include <iostream>
#include <string>
using std::cout;
using std::string;

// One pass with three flags; a sign is legal only at the start or right after 'e'.
// T: O(n), S: O(1).
bool isNumber(const string& s) {
    int n = s.size();
    bool seenDigit = false;   // a digit in the current part (mantissa, then exponent)
    bool seenDot = false;
    bool seenExp = false;
    for (int i = 0; i < n; i++) {
        char c = s[i];
        if (c >= '0' && c <= '9') {
            seenDigit = true;
        } else if (c == '+' || c == '-') {
            if (i > 0 && s[i - 1] != 'e' && s[i - 1] != 'E') { return false; }
        } else if (c == '.') {
            if (seenDot || seenExp) { return false; }
            seenDot = true;
        } else if (c == 'e' || c == 'E') {
            // the mantissa needs a digit, and the exponent needs its own digit
            if (seenExp || !seenDigit) { return false; }
            seenExp = true;
            seenDigit = false;
        } else {
            return false;
        }
    }
    return seenDigit;
}

void test(const string& label, const string& s, bool want) {
    cout << label << (isNumber(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("signed-exp", "-1.5e+3", true);
    test("trailing-dot", "4.", true);
    test("lone-dot", ".", false);
    test("empty-exp", "1e", false);
}
