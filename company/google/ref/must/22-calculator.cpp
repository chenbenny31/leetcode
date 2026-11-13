// 22 · Basic Calculator II — LC 227, 772, 150
// Mother: select/05-stack/02-basic-calculator-ii.cpp
// (1) LC 227: evaluate non-negative ints w/ + - * / and spaces; / truncates toward 0
// (2) LC 772 + unary signs: also parens and unary + / - before a num or '('
// (3) simplify one-letter vars w/ + - and parens: "a-(b-(c+a))" -> "2a-b+c"
//
// clarify:
// input valid? -> yes
// empty string -> 0
// division -> truncates toward 0; no div by zero
// overflow -> int in (1), long long in (2)
// (3) output -> order a..z, coefficient 1 omitted, "0" if all cancel

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::abs;
using std::to_string;

// (1) + - * / w/o parens (LC 227)
// core idea: keep signed terms; + pushes num, - pushes -num, * and / change the last term
// op = operator BEFORE num; applied at each operator and at the last char
// T: O(n), S: O(n)
int calculate(const string& s) {
    // signed terms whose sum is the answer
    vector<int> terms;
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

// + / - push a new signed term, * and / change the last term
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

// evaluate from pos to the matching ')' or the end; pos ends just past the ')'
long long evalFrom(const string& s, int& pos) {
    // signed terms whose sum is the value of this level
    vector<long long> terms;
    long long num = 0;
    char op = '+';
    // sign of the next number, from unary + / -
    int sign = 1;
    // true at start, after '(' and after an operator: a + / - here is unary
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

// (2) parens + unary signs (LC 772)
// core idea: same stack of terms; '(' recurses w/ shared pos (by ref), value = the num
// + / - is unary only where a num is expected
// T: O(n), S: O(n)
long long calculateWithParens(const string& s) {
    int pos = 0;
    return evalFrom(s, pos);
}

// (3) simplify variables
// core idea: stack of signs; term sign = its own + / - times sign of enclosing parens
// at '(' push the current sign, which already includes every outer paren
// T: O(n), S: O(n)
string simplifyVariables(const string& s) {
    // coef[letter] = signed count of that variable so far
    vector<int> coef(26, 0);
    // signs[d] = sign applied to everything inside paren depth d
    vector<int> signs = {1};
    // sign of the next term
    int sign = 1;
    for (char c : s) {
        if (c == '+') {
            sign = signs.back();
        } else if (c == '-') {
            sign = -signs.back();
        } else if (c == '(') {
            signs.push_back(sign);
        } else if (c == ')') {
            signs.pop_back();
        } else if (c >= 'a' && c <= 'z') {
            coef[c - 'a'] += sign;
        }
    }
    string result;
    for (int letter = 0; letter < 26; letter++) {
        int value = coef[letter];
        if (value == 0) { continue; }
        if (value < 0) {
            result += '-';
        } else if (!result.empty()) {
            result += '+';
        }
        int magnitude = abs(value);
        if (magnitude != 1) { result += to_string(magnitude); }
        result += (char)('a' + letter);
    }
    return result.empty() ? "0" : result;
}

void test(const string& label, const string& s, int want) {
    cout << label << (calculate(s) == want ? " PASS\n" : " FAIL\n");
}

void testParens(const string& label, const string& s, long long want) {
    cout << label << (calculateWithParens(s) == want ? " PASS\n" : " FAIL\n");
}

void testVars(const string& label, const string& s, const string& want) {
    cout << label << (simplifyVariables(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("precedence", "2+3*4", 14);
    test("spaces", " 6 / 3 * 2 ", 4);
    test("minus-divide", "1-7/2", -2);
    test("empty", "", 0);
    testParens("parens-nested", "2*(3+(4-1))", 12);
    testParens("parens-unary", "-(2+3)*-2", 10);
    testParens("parens-truncate", "(1-8)/2", -3);
    testVars("vars-nested-flip", "a-(b-(c+a))", "2a-b+c");
    testVars("vars-leading-minus", "-(a-b)", "-a+b");
    testVars("vars-cancel", "b-(b)", "0");
}

// follow-up:
// O(1) extra space for (1) -> keep only last term + running result; return result + last
// evaluate RPN (LC 150) -> operand stack, pop right then left; "-3" is a num
// more precedence levels -> shunting-yard: nums + ops stacks, reduce while top binds >=
// right-associative ^ -> reduce only while top binds strictly tighter
// nested function calls -> 23; RPN to infix -> 85
