// RPN to Infix (Google, interview report), two parts.
// rpn = comma-separated tokens, e.g. "2,1,+,3,*"; operands = ints >= 0, binary + - * /
// (1) fully parenthesized: every operation wrapped -> "((2+1)*3)"
// (2) minimal parentheses: only those needed to keep the meaning -> "(2+1)*3"
//
// clarify:
// spaces around tokens / in the output -> none
// multi-digit or negative operands -> multi-digit yes, negative no
// unary operators -> no, all binary
// invalid input -> assume valid
// '/' exact or integer -> exact; integer division = follow-up

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <cctype>

using std::cout;
using std::string;
using std::vector;
using std::stack;
using std::isdigit;

// split s on ',' w/ a plain char loop
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

// (1) fully parenthesized
// core idea: RPN = post-order of the expression tree -> stack of finished sub-expressions
// operator pops right, then left, pushes "(left op right)"
// T: O(n^2), S: O(n) — string copies
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

// 1 for + -, 2 for * /
int precedence(char op) {
    return (op == '+' || op == '-') ? 1 : 2;
}

// (2) minimal parentheses
// core idea: also keep each sub-expression's top operator precedence
// wrap a side binding looser than op; right side also on a tie when op is '-' or '/'
// T: O(n^2), S: O(n) — string copies
string rpnToMinimalInfix(const string& rpn) {
    // finished sub-expressions w/ their precedence, top = most recent
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
        // '-' and '/' not associative: a - (b - c) keeps its parentheses
        bool notAssociative = op == '-' || op == '/';
        bool wrapRight = right.prec < prec || (right.prec == prec && notAssociative);
        string rightText = wrapRight ? "(" + right.text + ")" : right.text;
        exprs.push({leftText + op + rightText, prec});
    }
    return exprs.top().text;
}

// follow-up: infix -> RPN (shunting-yard)
// core idea: numbers go straight to output; '(' is pushed, ')' pops down to its '('
// operator first pops stacked operators binding >= as tight (left assoc), then is pushed
// T: O(len), S: O(len)
string infixToRpn(const string& infix) {
    int n = infix.size();
    // output tokens in RPN order
    vector<string> out;
    // operators and '(' not output yet, innermost on top
    stack<char> ops;
    int pos = 0;
    while (pos < n) {
        char ch = infix[pos];
        if (isdigit(ch)) {
            int start = pos;
            while (pos < n && isdigit(infix[pos])) { pos++; }
            out.push_back(infix.substr(start, pos - start));
            continue;
        }
        pos++;
        if (ch == ' ') { continue; }
        if (ch == '(') {
            ops.push(ch);
        } else if (ch == ')') {
            while (ops.top() != '(') {
                out.push_back(string(1, ops.top()));
                ops.pop();
            }
            ops.pop();
        } else {
            while (!ops.empty() && ops.top() != '(' &&
                   precedence(ops.top()) >= precedence(ch)) {
                out.push_back(string(1, ops.top()));
                ops.pop();
            }
            ops.push(ch);
        }
    }
    while (!ops.empty()) {
        out.push_back(string(1, ops.top()));
        ops.pop();
    }
    string rpn;
    for (int i = 0; i < (int)out.size(); i++) {
        if (i > 0) { rpn += ','; }
        rpn += out[i];
    }
    return rpn;
}

void test(const string& label, const string& rpn, const string& wantFull,
          const string& wantMinimal) {
    bool ok = rpnToFullInfix(rpn) == wantFull && rpnToMinimalInfix(rpn) == wantMinimal;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

// also converts want back: minimal infix -> RPN -> minimal infix = identity
void testToRpn(const string& label, const string& infix, const string& want) {
    bool ok = infixToRpn(infix) == want && rpnToMinimalInfix(want) == infix;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("wrap-left", "1,2,+,3,*", "((1+2)*3)", "(1+2)*3");
    test("wrap-right", "8,1,3,+,/", "(8/(1+3))", "8/(1+3)");
    test("minus-right", "12,4,2,-,-", "(12-(4-2))", "12-(4-2)");
    test("mul-over-div", "6,4,2,/,*", "(6*(4/2))", "6*4/2");

    testToRpn("to-rpn-precedence", "1+2*3", "1,2,3,*,+");
    testToRpn("to-rpn-parens", "(1+2)*3", "1,2,+,3,*");
    testToRpn("to-rpn-left-assoc", "8-4-2", "8,4,-,2,-");
    testToRpn("to-rpn-nested", "12/(4-(1+1))", "12,4,1,1,+,-,/");
}

// follow-up:
// integer division -> a * (b / c) != a * b / c: store the top operator in Expr
// integer division wrap rule -> also wrap right side if op is '*' and its top op is '/'
// evaluate instead (LC 150) -> same stack holding numbers: pop b, then a, push a op b
// infix to RPN -> infixToRpn above
// pop only on > instead of >= -> 8-4-2 becomes 8,4,2,-,- = 8-(4-2)
// invalid RPN -> stack needs >= 2 entries before an operator pops, exactly 1 at the end
