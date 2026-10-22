// LC 227 Basic Calculator II: evaluate a valid expression of non-negative integers,
// '+', '-', '*', '/' and spaces. Integer division truncates toward zero. No eval().
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Keep a stack of signed terms: '*' and '/' fold into the top term, '+' and '-' push a
// new one. The answer is the sum of the terms. T: O(n), S: O(n).
int calculate(const string& s) {
    int n = s.size(), num = 0;
    char op = '+';          // operator waiting in front of num
    vector<int> terms;      // signed terms of the sum
    for (int i = 0; i < n; i++) {
        char c = s[i];
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
        }
        // the last char always flushes num, whether it is a digit or a trailing space
        if ((c != ' ' && (c < '0' || c > '9')) || i == n - 1) {
            if (op == '+') {
                terms.push_back(num);
            } else if (op == '-') {
                terms.push_back(-num);
            } else if (op == '*') {
                terms.back() *= num;
            } else {
                terms.back() /= num;
            }
            op = c;
            num = 0;
        }
    }
    int sum = 0;
    for (int t : terms) {
        sum += t;
    }
    return sum;
}

void test(const string& label, const string& s, int want) {
    cout << label << (calculate(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("precedence", "2+3*4-6/2", 11);
    test("spaces", " 12 / 5 * 5 ", 10);
    test("trunc-division", "7-8/3", 5);
    test("single-number", "42", 42);
}
