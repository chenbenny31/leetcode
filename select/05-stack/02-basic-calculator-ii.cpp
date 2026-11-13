// LC 227 Basic Calculator II (Medium) — stack of signed terms
// evaluate valid string of non-negative ints, + - * / and spaces; / truncates toward 0
//
// clarify:
// input valid? -> yes, no error handling
// parentheses / unary sign -> none (follow-up, LC 224 / 772)
// division -> truncates toward 0, same as C++ int /
// overflow -> assume every intermediate fits in int
// empty / all-space string -> 0

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// core idea: * and / only change the last term, so keep signed terms
// + pushes num, - pushes -num, * and / update the back; answer = sum of terms
// T: O(n), S: O(n)
int calculate(const string& s) {
    // terms = signed terms whose sum is the answer
    vector<int> terms;
    // num = number being read
    int num = 0;
    // op = operator before num
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

// follow-up:
// O(1) extra space -> keep result + last; + / - folds last into result
// parens and unary signs (LC 224 / 772) -> recursion w/ shared pos; '(' runs to its ')'
// simplify "a-(b-(c+a))" to "2a-b+c" -> stack of signs per open paren; coef per letter
// evaluate RPN (LC 150) -> operand stack; operator pops right, then left, pushes result
// more precedence levels (^) -> shunting-yard: nums stack + ops stack
// nested calls add(1, mul(2, 3)) -> company/google/ref/must/21-nested-function-eval
// RPN to infix -> company/google/ref/recommended/79-rpn-to-infix
// trailing operator, division by 0 -> settle the rule first, reject at the parse step
