// ByteDance (hack2hire) / LC 227: evaluate a valid string of non-negative integers,
// + - * / and spaces; integer division truncates toward zero. No parentheses.
// Follow-up: O(1) extra space.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// One pass: push each term with its sign; * and / apply to the last term at once.
// T: O(n), S: O(n).
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
    for (int t : terms) {
        result += t;
    }
    return result;
}

// Follow-up: keep only the last term instead of the stack. T: O(n), S: O(1).
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
    test("precedence", "3+2*2", 7);
    test("truncate", " 3/2 ", 1);
    test("spaces", " 3+5 / 2 ", 5);
    test("minus-divide", "1-7/2", -2);
    test("chain", "2*3*4-10/3", 21);
}

// follow-up: O(1) extra space -> see calculateO1() above; keep only the last term, fold
//   it into result on + / -
// follow-up: parentheses (LC 224/772) -> on '(' recurse with a shared index i, use the
//   returned value as num; ')' ends the recursive call like the end of the string
//   if (c == '(') { i++; num = calc(s, i); }
// follow-up: general precedence / unary minus -> shunting-yard: nums + ops stacks, pop
//   while top precedence >= cur, O(n)
