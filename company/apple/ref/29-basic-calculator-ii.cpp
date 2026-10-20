// LC 227: evaluate a valid string of non-negative integers, + - * / and spaces;
// integer division truncates toward zero. No parentheses.
// Follow-up (LC 772): the string may also contain ( and ).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// straightforward: one pass; push each term with its sign, apply * and / to the last
// term at once. T: O(n), S: O(n).
int calculate(const string& s) {
    vector<int> terms;   // signed terms whose sum is the answer
    int num = 0;
    char op = '+';       // operator before num
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
    for (int t : terms) { result += t; }
    return result;
}

// improved: only the last term can still change, so keep it instead of the stack.
// T: O(n), S: O(1).
int calculateO1(const string& s) {
    int result = 0;   // sum of finished terms
    int last = 0;     // current signed term, still open to * and /
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

// follow-up (LC 772): parentheses. '(' evaluates the inner expression recursively into
// one number; ')' or the end closes the current level. i = next char to read, shared
// by all levels. T: O(n), S: O(n) terms + recursion depth.
int evalExpr(const string& s, int& i) {
    vector<int> terms;   // signed terms of this level; their sum is its value
    int num = 0;
    char op = '+';       // operator before num
    int n = s.size();
    while (i < n) {
        char c = s[i];
        i++;
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
        } else if (c == '(') {
            // returns with i just past the matching ')'
            num = evalExpr(s, i);
        }
        bool isOp = c == '+' || c == '-' || c == '*' || c == '/';
        if (isOp || c == ')' || i == n) {
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
            if (c == ')') { break; }
        }
    }
    int result = 0;
    for (int t : terms) { result += t; }
    return result;
}

int calculateParen(const string& s) {
    int i = 0;
    return evalExpr(s, i);
}

void test(const string& label, const string& s, int want) {
    bool ok = calculate(s) == want && calculateO1(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("prec", "1+2*3", 7);
    test("spaces", " 3+5 / 2 ", 5);
    test("trunc", "1-3*2/4", 0);
}
