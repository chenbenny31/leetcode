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

// apply op to num: + / - push a new signed term, * and / change the last term
void applyOp(vector<long long>& terms, char op, long long num) {
    if (op == '+') {
        terms.push_back(num);
    } else if (op == '-') {
        terms.push_back(-num);
    } else if (op == '*') {
        terms.back() *= num;
    } else {
        terms.back() /= num;
    }
}

// evaluate from pos up to the matching ')' or the end; pos ends just past the ')'
long long evalFrom(const string& s, int& pos) {
    // signed terms whose sum is the value of this level
    vector<long long> terms;
    long long num = 0;
    char op = '+';
    // sign of the next number, from unary + / -
    int sign = 1;
    // true at the start, after '(' and after an operator: a + / - here is unary
    bool expectNum = true;
    int n = s.size();
    while (pos < n) {
        char c = s[pos];
        pos++;
        if (c == ' ') { continue; }
        if (c == ')') { break; }
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
            expectNum = false;
        } else if (c == '(') {
            // the whole group acts as one number
            num = evalFrom(s, pos);
            expectNum = false;
        } else if (expectNum) {
            if (c == '-') { sign = -sign; }
        } else {
            applyOp(terms, op, sign * num);
            op = c;
            num = 0;
            sign = 1;
            expectNum = true;
        }
    }
    applyOp(terms, op, sign * num);
    long long result = 0;
    for (long long term : terms) { result += term; }
    return result;
}

// Google (Basic Calculator III, LC 772 + unary signs): + - * /, parentheses, spaces,
// unary + / - before a number or '('; / truncates toward 0. Assumes valid input.
// Same stack of terms; '(' recurses with a shared pos and its value is the number.
// T: O(n), S: O(n).
long long calculateWithParens(const string& s) {
    int pos = 0;
    return evalFrom(s, pos);
}

void test(const string& label, const string& s, int want) {
    bool ok = calculate(s) == want && calculateO1(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testParens(const string& label, const string& s, long long want) {
    cout << label << (calculateWithParens(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("precedence", "2+3*4", 14);
    test("spaces", " 6 / 3 * 2 ", 4);
    test("minus-divide", "1-7/2", -2);
    test("multi-digit", "12-10/3+40", 49);
    testParens("parens-nested", "2*(3+(4-1))", 12);
    testParens("parens-unary", "-(2+3)*-2", 10);
    testParens("parens-truncate", "(1-8)/2", -3);
    testParens("parens-spaces", " 3 - -2 * ( 1 + 1 ) ", 7);
}

// follow-up: general precedence / unary minus -> shunting-yard with nums + ops stacks
// follow-up: edge cases (empty / spaces only -> 0, leading unary sign, trailing operator,
//   division by zero -> error) -> settle each rule with the interviewer first, then
//   handle it at the parse step
