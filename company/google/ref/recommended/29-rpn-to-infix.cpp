// RPN to Infix (Google, interview report), two parts.
// rpn = comma-separated tokens, e.g. "2,1,+,3,*". Operands are non-negative integers,
// operators are binary + - * /, input is assumed valid; '/' is exact (not integer)
// division.
// (1) fully parenthesized: every operation wrapped -> "((2+1)*3)".
// (2) minimal parentheses: only those needed to keep the meaning -> "(2+1)*3".

#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::cout;
using std::string;
using std::vector;
using std::stack;

// splits s on ',' with a plain character loop
vector<string> splitTokens(const string& s) {
    vector<string> tokens;
    string token;
    for (char ch : s) {
        if (ch == ',') {
            tokens.push_back(token);
            token.clear();
        } else {
            token += ch;
        }
    }
    tokens.push_back(token);
    return tokens;
}

bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

// (1) stack of sub-expressions; an operator pops right, then left, and pushes
// "(left op right)". T: O(n^2) worst for string copies, S: O(n).
string rpnToFullInfix(const string& rpn) {
    // finished sub-expressions, top = most recent
    stack<string> exprs;
    for (const string& token : splitTokens(rpn)) {
        if (!isOperator(token)) {
            exprs.push(token);
            continue;
        }
        string right = exprs.top();
        exprs.pop();
        string left = exprs.top();
        exprs.pop();
        exprs.push("(" + left + token + right + ")");
    }
    return exprs.top();
}

struct Expr {
    string text;
    // 1 for + -, 2 for * /, 3 for a number (never wrapped)
    int prec;
};

int precedence(char op) {
    return (op == '+' || op == '-') ? 1 : 2;
}

// (2) also remember each sub-expression's top operator precedence; wrap a side only when
// it binds looser than op. T: O(n^2) worst for string copies, S: O(n).
string rpnToMinimalInfix(const string& rpn) {
    // finished sub-expressions with their precedence, top = most recent
    stack<Expr> exprs;
    for (const string& token : splitTokens(rpn)) {
        if (!isOperator(token)) {
            exprs.push({token, 3});
            continue;
        }
        Expr right = exprs.top();
        exprs.pop();
        Expr left = exprs.top();
        exprs.pop();
        char op = token[0];
        int prec = precedence(op);
        string leftText = left.prec < prec ? "(" + left.text + ")" : left.text;
        // '-' and '/' are not associative: a - (b - c) keeps its parentheses
        bool notAssociative = op == '-' || op == '/';
        bool wrapRight = right.prec < prec || (right.prec == prec && notAssociative);
        string rightText = wrapRight ? "(" + right.text + ")" : right.text;
        exprs.push({leftText + op + rightText, prec});
    }
    return exprs.top().text;
}

void test(const string& label, const string& rpn, const string& wantFull,
          const string& wantMinimal) {
    bool ok = rpnToFullInfix(rpn) == wantFull && rpnToMinimalInfix(rpn) == wantMinimal;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("wrap-left", "1,2,+,3,*", "((1+2)*3)", "(1+2)*3");
    test("plus-chain", "1,2,+,3,+", "((1+2)+3)", "1+2+3");
    test("wrap-right", "8,1,3,+,/", "(8/(1+3))", "8/(1+3)");
    test("minus-right", "12,4,2,-,-", "(12-(4-2))", "12-(4-2)");
    test("minus-left", "12,4,-,2,-", "((12-4)-2)", "12-4-2");
}

// follow-up: integer division -> a * (b / c) != a * b / c, so also wrap the right side
//   when op is '*' and the right side's top operator is '/' (store the op, not just prec)
// follow-up: evaluate instead -> same stack holding numbers; pop right, then left, apply
// follow-up: infix to RPN -> shunting-yard with an operator stack
