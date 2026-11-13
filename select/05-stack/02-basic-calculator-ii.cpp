// LC 227 Basic Calculator II (Medium) — stack of signed terms
// Evaluate a valid string of non-negative ints, + - * / and spaces; / truncates toward
// 0. No parentheses.
// Clarify: input always valid; no parentheses or unary sign; / truncates toward 0;
//   every intermediate fits in int; empty or all-space string.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// Core idea: * and / only ever change the last term, so keep signed terms: + pushes num,
// - pushes -num, * and / update the back; the answer is their sum. T: O(n), S: O(n).
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

void test(const string& label, const string& s, int want) {
    cout << label << (calculate(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("precedence", "2+3*4", 14);
    test("spaces", " 6 / 3 * 2 ", 4);
    test("minus-divide", "1-7/2", -2);
    test("multi-digit", "12-10/3+40", 49);
    return 0;
}

// follow-up (ideas only):
// - O(1) extra space -> only the last term can still change: keep result + last; a + or
//   - adds last into result and starts a new last; return result + last
// - parentheses and unary signs (LC 224 / 772) -> recursion with a shared pos: '('
//   evaluates up to its ')' and the value is the number; + / - is unary where a number
//   is expected
// - simplify a variable expression "a-(b-(c+a))" -> "2a-b+c" -> stack of signs, one per
//   open paren; a term's sign = its own sign times signs.back(); coef per letter
// - evaluate RPN (LC 150) -> stack of operands; an operator pops right, then left, and
//   pushes left op right; "-3" is a number, only a 1-char "-" is an operator
// - more precedence levels (^, right-associative) -> shunting-yard: a nums stack and an
//   ops stack; before pushing an operator reduce while the top binds at least as tight
// - nested calls add(1, mul(2, 3)); RPN to infix -> company/google/ref/must/21-nested-
//   function-eval, company/google/ref/recommended/79-rpn-to-infix
// - edge cases -> empty / spaces only gives 0 and a leading '-' reads as 0 - x; trailing
//   operator, division by zero: settle the rule first, then reject at the parse step
