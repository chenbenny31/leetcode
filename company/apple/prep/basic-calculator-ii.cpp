// eval a valid string of non-negative ints, +-*/ and spaces
// integer division truncates towards 0, no parentheses

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// one pass with stack storing valus, record last op, T: O(n), S: O(n)
int calculate(const string& s) {
    vector<int> terms; // signed terms sum to answer
    int num = 0;
    char op = '+';
    const int n = s.size();

    for (int i = 0; i < n; i++) {
        char c = s[i];
        bool isDigit = (c >= '0' && c <= '9');
        if (isDigit) { num = num * 10 + (c - '0'); }
        // apply op: c is an op OR last i, last i could still be digit
        if ((!isDigit && c != ' ') || i == n - 1) {
            if (op == '+') {
                terms.push_back(num);
            } else if (op == '-') {
                terms.push_back(-num);
            } else if (op == '*') {
                terms.back() *= num;
            } else if (op == '/') {
                terms.back() /= num;
            }
            op = c;
            num = 0;
        }
    }
    int result = 0;
    for (int t : terms) { result += t; }
    return result;
}

void test(const string& label, const string& s, int want) {
    bool ok = calculate(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("prec", "1+2*3", 7);
    test("spaces", " 3+5 / 2 ", 5);
    test("trunc", "1-3*2/4", 0);
}
