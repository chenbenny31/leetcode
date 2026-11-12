// Evaluate Nested Function-Call Expression (Google, interview report): evaluate a string
// such as "add(1, mul(2, sub(5, 3)))". Functions: add, sub, mul, div, pow, each with
// exactly two arguments. An argument is an integer literal (maybe negative, e.g. -4) or a
// nested call. Spaces may appear around arguments. The input is well-formed; div
// truncates toward zero (C++ integer division, divisor != 0); pow takes an exponent >= 0.
//
// Clarify:
// - grammar: arg := integer | name '(' arg ',' arg ')'; names add sub mul div pow, two
//   args each (stated); unknown name -> assume none
// - literals: optional '-' then digits; a leading '+'? -> assume none
// - spaces around args (stated); tabs or newlines? -> assume spaces only
// - well-formed (stated); malformed -> report an error (follow-up)
// - div truncates toward zero, divisor != 0; pow exponent >= 0 (stated)
// - range -> every value fits in long long (assume); nesting depth = recursion depth
//   (iterative follow-up for very deep input)
// Idea: recursive descent: one function parses one arg, and the args of a call are args
//   again, so it recurses into them. All calls share one position index by reference;
//   each leaves it just past what it consumed, so every char is read once: O(len).
// Pitfalls:
// - int& pos: passed by value, the caller re-reads the chars the callee consumed
// - skip spaces before every token, also before ',' and ')' ("div( pow(2, 10) , 4 )")
// - '-' starts a number here (there is no binary minus): test it before the name branch,
//   then read the digits after it
// - stoll, not stoi: "3000000000" overflows int
// - pow: result starts at 1 so pow(x, 0) = 1; div: C++ '/' truncates (-7 / 2 = -3), it
//   does not floor
// Tests to write: bare literal; flat call; nested on both sides; negative literals;
//   spaces around args; no spaces; div truncates toward zero; pow(x, 0); a result past
//   int.

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using std::cout;
using std::string;
using std::vector;
using std::stoll;
using std::isdigit;
using std::isalpha;

// moves pos past any spaces. T: O(spaces), S: O(1).
void skipSpaces(const string& s, int& pos) {
    int n = s.size();
    while (pos < n && s[pos] == ' ') { pos++; }
}

// applies one function to its two values; pow by repeated multiplication.
// T: O(1), O(b) for pow, S: O(1).
long long apply(const string& name, long long a, long long b) {
    if (name == "add") { return a + b; }
    if (name == "sub") { return a - b; }
    if (name == "mul") { return a * b; }
    if (name == "div") { return a / b; }
    long long result = 1;
    for (long long i = 0; i < b; i++) { result *= a; }
    return result;
}

// recursive descent with one shared pos: arg := integer | name '(' arg ',' arg ')'
// T: O(len) (pow adds O(exponent)), S: O(nesting depth) for the recursion.
long long parseArg(const string& s, int& pos) {
    int n = s.size();
    skipSpaces(s, pos);
    if (s[pos] == '-' || isdigit(s[pos])) {
        int start = pos;
        pos++;
        while (pos < n && isdigit(s[pos])) { pos++; }
        return stoll(s.substr(start, pos - start));
    }
    int start = pos;
    while (isalpha(s[pos])) { pos++; }
    string name = s.substr(start, pos - start);
    // skip '('
    pos++;
    long long a = parseArg(s, pos);
    skipSpaces(s, pos);
    // skip ','
    pos++;
    long long b = parseArg(s, pos);
    skipSpaces(s, pos);
    // skip ')'
    pos++;
    return apply(name, a, b);
}

// parses the whole string as one arg. T: O(len), S: O(depth).
long long evaluate(const string& s) {
    int pos = 0;
    return parseArg(s, pos);
}

// an open call in evaluateIterative: its name and the argument values parsed so far
struct Frame {
    string name;
    vector<long long> args;
};

// follow-up: no recursion (very deep nesting). A stack of open calls: a name opens a
// frame; a number, or a ')' closing the top frame, gives a value that goes to the
// innermost frame still open (or is the answer at top level). '(' ',' and spaces carry
// no information.
// T: O(len), S: O(depth).
long long evaluateIterative(const string& s) {
    int n = s.size();
    // open calls, innermost last
    vector<Frame> frames;
    long long result = 0;
    int pos = 0;
    while (pos < n) {
        char c = s[pos];
        if (isalpha(c)) {
            int start = pos;
            while (pos < n && isalpha(s[pos])) { pos++; }
            frames.push_back({s.substr(start, pos - start), {}});
            continue;
        }
        if (c != ')' && c != '-' && !isdigit(c)) {
            pos++;
            continue;
        }
        long long value = 0;
        if (c == ')') {
            pos++;
            Frame top = frames.back();
            frames.pop_back();
            value = apply(top.name, top.args[0], top.args[1]);
        } else {
            int start = pos;
            pos++;
            while (pos < n && isdigit(s[pos])) { pos++; }
            value = stoll(s.substr(start, pos - start));
        }
        if (frames.empty()) {
            result = value;
        } else {
            frames.back().args.push_back(value);
        }
    }
    return result;
}

void test(const string& label, const string& expr, long long want) {
    bool ok = evaluate(expr) == want && evaluateIterative(expr) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("literal", "42", 42);
    test("flat", "add(1, 2)", 3);
    test("nested", "sub(mul(2, 3), add(1, 1))", 4);
    test("negative", "mul(-3, add(-2,10))", -24);
    test("div-pow", "div( pow(2, 10) , 4 )", 256);
    test("neg-literal", "-7", -7);
    test("div-no-spaces", "div(sub(-9,3),mul(2,-3))", 2);
    test("long-long", "mul(3000000000,mul(2,-2))", -12000000000LL);
    test("div-truncate", "div(-7, 2)", -3);
    test("pow-zero", "pow(5, 0)", 1);
}

// follow-up: variadic, left-associative (add(1, 2, 3)) -> replace the two-arg reads:
//   long long acc = parseArg(s, pos);
//   skipSpaces(s, pos);
//   while (s[pos] == ',') {
//       pos++;
//       acc = apply(name, acc, parseArg(s, pos));
//       skipSpaces(s, pos);
//   }
//   then skip ')' as before; zero args "add()" -> test s[pos] == ')' first
// follow-up: iterative instead of recursive -> evaluateIterative above. Same O(len); no
//   call-stack overflow on very deep nesting.
// follow-up: malformed input -> check pos < n before every s[pos] read, and turn each
//   blind pos++ over '(' ',' ')' into a checked one, reporting through a bool& ok:
//   if (pos >= n || s[pos] != '(') { ok = false; return 0; }
//   pos++;
//   also: a lone "-", an unknown name, div by 0, text left over after evaluate
// follow-up: huge exponent -> the pow loop is O(b); square-and-multiply is O(log b):
//   while (b > 0) { if (b % 2 == 1) { result *= a; } b /= 2; if (b > 0) { a *= a; } }
//   (the b > 0 test skips a last squaring that could overflow)
// follow-up: add an operator cheaply -> one more line in apply, or a map from name to a
//   function; the parser does not change.
// follow-up: tokenizer first -> split into name / '(' / ')' / ',' / number tokens, then
//   the same recursion walks tokens instead of characters (cleaner whitespace handling).
// follow-up: LC 3749 Evaluate Valid Expressions -> this code as is: add / sub / mul /
//   div, negative literals, no spaces, long long results, every div exact
