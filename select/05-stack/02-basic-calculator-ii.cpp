// LC 227 Basic Calculator II (Medium). Pattern: stack of signed terms.
// Evaluate a valid string of non-negative ints, + - * / and spaces; / truncates toward 0.
// No parentheses.
//
// Clarify:
// - valid input? -> yes: non-negative ints, + - * /, spaces; no parentheses, no unary
//   sign (a leading '-' happens to work: it reads as 0 - x)
// - / truncates toward 0 -> C++ int / already does (Python's // floors)
// - result and every intermediate fit in int? -> LC guarantees it; else long long
// - length up to 3e5 -> one pass, O(n)
// - empty or only spaces -> 0
// Idea: * and / bind tighter, so they only ever change the last term. Keep signed
//   terms: + pushes num, - pushes -num, * and / change terms.back(); the answer is
//   their sum. The pending op is applied when the next operator (or the end) shows up.
// Pitfalls:
// - op is the operator BEFORE num: apply it, then op = c and num = 0
// - the last number has no operator after it: test i == n - 1 in the same if, after
//   the digit was added to num
// - a space must not trigger the apply (c != ' '), or op becomes ' '
// - num * 10 + (c - '0'): without the parentheses num * 10 + c can overflow first
// - terms.back() for * and / always exists: op starts as '+', so the first number is
//   pushed before any * or /
// Tests to write: * before +; spaces everywhere incl. a trailing one; minus then divide
//   (truncation); multi-digit numbers; empty; leading minus.

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::abs;
using std::to_string;

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
// Pitfall: the last term is never added inside the loop; return result + last.
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
// Pitfalls: pos by reference, so the caller resumes after the ')'; + / - is unary only
//   where a number is expected (start, after '(' or an operator); apply the pending op
//   at ')' and at the end. T: O(n), S: O(n).
long long calculateWithParens(const string& s) {
    int pos = 0;
    return evalFrom(s, pos);
}

// Google (×2): simplify an expression of one-letter variables, + -, parentheses and
// spaces, e.g. "a-(b-(c+a))" -> "2a-b+c". Output: letters a..z, coefficient 1 omitted,
// a leading + omitted, zero terms dropped, "0" if everything cancels.
// Stack of signs: signs.back() = sign the enclosing parentheses put on their contents;
// a term's sign = its own + / - times that. T: O(n), S: O(n).
// Pitfalls: at '(' push sign, which already includes every enclosing paren (pushing
//   only the + / - right before it loses the outer flips); a '-' right after '(' still
//   flips, via signs.back(); drop zero coefficients; print "0" for an empty result.
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
    bool ok = calculate(s) == want && calculateO1(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
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
    test("multi-digit", "12-10/3+40", 49);
    test("empty", "", 0);
    test("leading-minus", "-3*2+1", -5);
    testParens("parens-nested", "2*(3+(4-1))", 12);
    testParens("parens-unary", "-(2+3)*-2", 10);
    testParens("parens-truncate", "(1-8)/2", -3);
    testParens("parens-spaces", " 3 - -2 * ( 1 + 1 ) ", 7);
    testVars("vars-nested-flip", "a-(b-(c+a))", "2a-b+c");
    testVars("vars-leading-minus", "-(a-b)", "-a+b");
    testVars("vars-cancel", "b-(b)", "0");
    testVars("vars-minus-inside", " b + b - ( - c ) ", "2b+c");
}

// follow-up: O(1) extra space -> calculateO1 above
// follow-up: parentheses + unary signs (LC 772, Google ×3) -> calculateWithParens above
// follow-up: variable expression (Google ×2) -> simplifyVariables above
// follow-up: nested calls add(1, mul(2, 3)) -> company/google/ref/recommended/14-nested-
//   function-eval.cpp; RPN to infix -> 29-rpn-to-infix.cpp there
// follow-up: more precedence levels (^, right-associative) -> shunting-yard with a nums
//   stack and an ops stack; before pushing operator c, reduce while the top binds at
//   least as tight (strictly tighter when c is the right-associative ^):
//     while (!ops.empty() && ops.back() != '(' && (prec[ops.back()] > prec[c]
//            || (prec[ops.back()] == prec[c] && c != '^'))) {
//         reduce(nums, ops);
//     }
//     ops.push_back(c);
//   '(' pushes; ')' reduces down to '(' and pops it; at the end reduce everything
// follow-up: edge cases -> empty / spaces only -> 0 and a leading '-' already work
//   (tests empty, leading-minus); trailing operator, division by zero -> settle each
//   rule with the interviewer first, then reject it at the parse step
