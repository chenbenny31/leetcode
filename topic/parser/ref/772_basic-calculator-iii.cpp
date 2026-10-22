// LC 772 Basic Calculator III: evaluate a valid expression of non-negative integers,
// '+', '-', '*', '/', '(', ')' and spaces. Integer division truncates toward zero.
// No eval().
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Recursive descent: the term-stack scan of LC 227, and '(' recurses to get the value of
// the group as one number. Returns with i just past the group's ')' (or at the end).
// T: O(n), S: O(n) for the recursion depth and the terms.
int calc(const string& s, int& i) {
    int n = s.size(), num = 0;
    char op = '+';          // operator waiting in front of num
    vector<int> terms;      // signed terms of the sum at this depth
    while (i < n) {
        char c = s[i];
        i++;
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
        } else if (c == '(') {
            num = calc(s, i);
        }
        // '(' is not an operator: its value waits in num for the next operator
        bool isOp = c == '+' || c == '-' || c == '*' || c == '/' || c == ')';
        if (isOp || i == n) {
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
            if (c == ')') { break; }
        }
    }
    int sum = 0;
    for (int t : terms) {
        sum += t;
    }
    return sum;
}

int calculate(const string& s) {
    int i = 0;
    return calc(s, i);
}

void test(const string& label, const string& s, int want) {
    cout << label << (calculate(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("precedence", "2+3*4-6/2", 11);
    test("paren-factor", "2*(3+4)*5", 70);
    test("nested", "10-(2*(3+4)-6/(1+2))", -2);
    test("paren-end", "9/(4-1)", 3);
}
