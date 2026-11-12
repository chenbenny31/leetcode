// RPN to Infix (Google, interview report), two parts.
// rpn = comma-separated tokens, e.g. "2,1,+,3,*". Operands are non-negative integers,
// operators are binary + - * /, input is assumed valid; '/' is exact (not integer)
// division.
// (1) fully parenthesized: every operation wrapped -> "((2+1)*3)".
// (2) minimal parentheses: only those needed to keep the meaning -> "(2+1)*3".
//
// Clarify:
// - tokens split by ',' with no spaces? -> assume so (else trim each token)
// - operands non-negative, maybe multi-digit; operators binary + - * / only (stated)
// - valid input (stated): one expression is left at the end; a single number "5" -> "5"
// - '/' is exact division (stated) -> a * (b / c) == a * b / c; integer division is a
//   follow-up
// - output has no spaces: "(2+1)*3"
// - up to ~1e4 tokens -> string copies make it O(n^2) worst; fine
// Idea: RPN is a post-order walk of the expression tree, so a stack of finished
//   sub-expressions rebuilds it: an operator pops its right, then its left operand and
//   pushes the joined string. (2) also keeps each sub-expression's top operator
//   precedence: wrap a side that binds looser than op, and the right side also when it
//   ties and op is '-' or '/' (they are not associative).
// Pitfalls:
// - the first pop is the RIGHT operand; swapping them turns 8/(1+3) into (1+3)/8
// - copy top() into a string before pop(), never a reference
// - (2) a tie on the right of '-' or '/' keeps its parentheses: 12-(4-2); a tie on the
//   left never needs them: (12-4)-2 -> 12-4-2
// - (2) a number gets precedence 3 so it is never wrapped; a new node's precedence is
//   op's own, not its children's
// - split: push the last token after the loop (no trailing ',')
// Tests to write: wrap the left side; a + chain needs none; wrap the right side; '-' with
//   a tie on the right vs on the left; '*' over '/' on the right needs none; one number.

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

// splits s on ',' with a plain character loop. T: O(len), S: O(len).
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

// true for the four binary operators. T: O(1), S: O(1).
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

// 1 for + -, 2 for * /. T: O(1), S: O(1).
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

// follow-up: infix -> RPN (shunting-yard). Numbers go straight to the output; an
// operator first pops every stacked operator that binds at least as tight (>= gives
// left associativity), then is pushed; '(' is pushed; ')' pops down to its '('.
// Input: non-negative integers, + - * /, parentheses, spaces; no unary minus.
// T: O(len), S: O(len).
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
            // drop the '('
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

// also converts want back: minimal infix -> RPN -> minimal infix is the identity
void testToRpn(const string& label, const string& infix, const string& want) {
    bool ok = infixToRpn(infix) == want && rpnToMinimalInfix(want) == infix;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("wrap-left", "1,2,+,3,*", "((1+2)*3)", "(1+2)*3");
    test("plus-chain", "1,2,+,3,+", "((1+2)+3)", "1+2+3");
    test("wrap-right", "8,1,3,+,/", "(8/(1+3))", "8/(1+3)");
    test("minus-right", "12,4,2,-,-", "(12-(4-2))", "12-(4-2)");
    test("minus-left", "12,4,-,2,-", "((12-4)-2)", "12-4-2");
    test("mul-over-div", "6,4,2,/,*", "(6*(4/2))", "6*4/2");
    test("div-over-mul", "8,2,4,*,/", "(8/(2*4))", "8/(2*4)");
    test("one-number", "7", "7", "7");

    testToRpn("to-rpn-precedence", "1+2*3", "1,2,3,*,+");
    testToRpn("to-rpn-parens", "(1+2)*3", "1,2,+,3,*");
    testToRpn("to-rpn-left-assoc", "8-4-2", "8,4,-,2,-");
    testToRpn("to-rpn-nested", "12/(4-(1+1))", "12,4,1,1,+,-,/");
}

// follow-up: integer division -> a * (b / c) != a * b / c, so also wrap the right side
//   when op is '*' and the right side's top operator is '/': store the op in Expr
//   (char op, ' ' for a number), then
//   bool wrapRight = right.prec < prec || (right.prec == prec &&
//                    (notAssociative || (op == '*' && right.op == '/')));
// follow-up: evaluate instead (LC 150) -> same stack holding numbers:
//   long long b = vals.top(); vals.pop(); long long a = vals.top(); vals.pop();
//   vals.push(op == '+' ? a + b : op == '-' ? a - b : op == '*' ? a * b : a / b);
// follow-up: infix to RPN -> infixToRpn above (shunting-yard with an operator stack).
//   Pitfall: pop while the top binds at least as tight (>=); with > the chain 8-4-2
//   comes out as 8,4,2,-,- = 8-(4-2)
// follow-up: invalid RPN -> check exprs.size() >= 2 before an operator pops and
//   exprs.size() == 1 at the end
