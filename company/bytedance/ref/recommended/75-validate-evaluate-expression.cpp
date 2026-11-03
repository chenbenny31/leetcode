// ByteDance (jobright, phone) LC 227 variant: s has digits, + - * /, parentheses, spaces.
// Return (true, value) if s is a valid expression, else (false, 0). Invalid: bad chars,
// unbalanced or empty parens, two binary operators in a row, leading / trailing binary
// operator, implicit multiplication like 2(3), or any division by zero (even 6/(3-3)).
// Unary + / - are allowed where an operand is expected. Division truncates toward zero.

#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::pair;

// Recursive descent; the grammar itself is the validity check:
//   expr   := term (('+' | '-') term)*
//   term   := factor (('*' | '/') factor)*
//   factor := ('+' | '-') factor | number | '(' expr ')'
// Each helper reads from s[pos] and sets ok = false on the first error.
// T: O(n), S: O(n) recursion depth for nested parentheses.

void skipSpaces(const string& s, int& pos) {
    while (pos < (int)s.size() && s[pos] == ' ') { pos++; }
}

long long parseExpr(const string& s, int& pos, bool& ok);

long long parseFactor(const string& s, int& pos, bool& ok) {
    skipSpaces(s, pos);
    if (pos == (int)s.size()) {
        ok = false;
        return 0;
    }
    char c = s[pos];
    if (c == '+' || c == '-') {
        pos++;
        long long value = parseFactor(s, pos, ok);
        if (c == '-') { return -value; }
        return value;
    }
    if (c == '(') {
        pos++;
        long long value = parseExpr(s, pos, ok);
        skipSpaces(s, pos);
        if (!ok || pos == (int)s.size() || s[pos] != ')') {
            ok = false;
            return 0;
        }
        pos++;
        return value;
    }
    if (c < '0' || c > '9') {
        ok = false;
        return 0;
    }
    long long value = 0;
    while (pos < (int)s.size() && s[pos] >= '0' && s[pos] <= '9') {
        value = value * 10 + (s[pos] - '0');
        pos++;
    }
    return value;
}

long long parseTerm(const string& s, int& pos, bool& ok) {
    long long value = parseFactor(s, pos, ok);
    while (ok) {
        skipSpaces(s, pos);
        if (pos == (int)s.size() || (s[pos] != '*' && s[pos] != '/')) { break; }
        char op = s[pos];
        pos++;
        long long rhs = parseFactor(s, pos, ok);
        if (!ok) { break; }
        if (op == '*') {
            value *= rhs;
        } else if (rhs == 0) {
            ok = false;
        } else {
            value /= rhs;
        }
    }
    return value;
}

long long parseExpr(const string& s, int& pos, bool& ok) {
    long long value = parseTerm(s, pos, ok);
    while (ok) {
        skipSpaces(s, pos);
        if (pos == (int)s.size() || (s[pos] != '+' && s[pos] != '-')) { break; }
        char op = s[pos];
        pos++;
        long long rhs = parseTerm(s, pos, ok);
        if (op == '+') {
            value += rhs;
        } else {
            value -= rhs;
        }
    }
    return value;
}

pair<bool, long long> validateAndEvaluate(const string& s) {
    int pos = 0;
    bool ok = true;
    long long value = parseExpr(s, pos, ok);
    skipSpaces(s, pos);
    // leftover input (e.g. "2(3)" or "1)") means the grammar did not match all of s
    if (!ok || pos != (int)s.size()) { return {false, 0}; }
    return {true, value};
}

void test(const string& label, const string& s, pair<bool, long long> want) {
    cout << label << (validateAndEvaluate(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("precedence", "2 + 3 * 4", {true, 14});
    test("unary-paren", "-(1+2)*3", {true, -9});
    test("div-zero-expr", "6/(3-3)", {false, 0});
    test("implicit-mul", "2(3)", {false, 0});
    test("double-op", "1+*2", {false, 0});
}
