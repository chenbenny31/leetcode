// LC 227 Basic Calculator II (Medium). Pattern: stack of signed terms.
// Evaluate a valid string of non-negative ints, + - * / and spaces; / truncates toward 0.
// No parentheses.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// Push each term with its sign; * and / apply to the last term at once. T: O(n), S: O(n).
int calculate(const string& s) {
    // signed terms whose sum is the answer
    vector<int> terms;
    // number being read
    int num = 0;
    // operator before num
    char op = '+';
    int n = s.size();
    for (int i = 0; i < n; i++) {
        char c = s[i];
        bool digit = c >= '0' && c <= '9';
        if (digit) { num = num * 10 + (c - '0'); }
        // apply the pending op at each operator, and once more at the end
        if ((!digit && c != ' ') || i == n - 1) {
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
    int result = 0;
    for (int term : terms) { result += term; }
    return result;
}

// Follow-up: only the last term can still change, keep it instead of the stack.
// T: O(n), S: O(1).
int calculateO1(const string& s) {
    // sum of finished terms
    int result = 0;
    // current signed term, still open to * and /
    int last = 0;
    int num = 0;
    char op = '+';
    int n = s.size();
    for (int i = 0; i < n; i++) {
        char c = s[i];
        bool digit = c >= '0' && c <= '9';
        if (digit) { num = num * 10 + (c - '0'); }
        if ((!digit && c != ' ') || i == n - 1) {
            if (op == '+' || op == '-') {
                // + or - closes the last term
                result += last;
                last = op == '+' ? num : -num;
            } else if (op == '*') {
                last *= num;
            } else {
                last /= num;
            }
            op = c;
            num = 0;
        }
    }
    return result + last;
}

void test(const string& label, const string& s, int want) {
    bool ok = calculate(s) == want && calculateO1(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("precedence", "2+3*4", 14);
    test("spaces", " 6 / 3 * 2 ", 4);
    test("minus-divide", "1-7/2", -2);
    test("multi-digit", "12-10/3+40", 49);
}

// follow-up: parentheses (LC 772) -> on '(' recurse with a shared index i and use the
//   returned value as num; ')' ends the call like the end of the string
// follow-up: general precedence / unary minus -> shunting-yard with nums + ops stacks
