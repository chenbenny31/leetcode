// Evaluate Nested Function-Call Expression (Google, interview report): evaluate a string
// such as "add(1, mul(2, sub(5, 3)))". Functions: add, sub, mul, div, pow, each with
// exactly two arguments. An argument is an integer literal (maybe negative, e.g. -4) or a
// nested call. Spaces may appear around arguments. The input is well-formed; div
// truncates toward zero (C++ integer division, divisor != 0); pow takes an exponent >= 0.

#include <iostream>
#include <string>
#include <cctype>

using std::cout;
using std::string;
using std::stoll;
using std::isdigit;
using std::isalpha;

void skipSpaces(const string& s, int& pos) {
    int n = s.size();
    while (pos < n && s[pos] == ' ') { pos++; }
}

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

long long evaluate(const string& s) {
    int pos = 0;
    return parseArg(s, pos);
}

void test(const string& label, const string& expr, long long want) {
    cout << label << (evaluate(expr) == want ? " PASS\n" : " FAIL\n");
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
}

// follow-up: variadic, left-associative (add(1, 2, 3)) -> after the first arg, loop:
//   while (s[pos] == ',') { pos++; acc = apply(name, acc, parseArg(s, pos));
//   skipSpaces(s, pos); }
// follow-up: iterative instead of recursive -> explicit stack of frames (name, args so
//   far); on ')' pop the frame, apply, and push the result into the parent frame's args.
//   Same O(len); no call-stack overflow on very deep nesting.
// follow-up: add an operator cheaply -> one more line in apply, or a map from name to a
//   function; the parser does not change.
// follow-up: tokenizer first -> split into name / '(' / ')' / ',' / number tokens, then
//   the same recursion walks tokens instead of characters (cleaner whitespace handling).
// follow-up: LC 3749 Evaluate Valid Expressions -> this code as is: add / sub / mul /
//   div, negative literals, no spaces, long long results, every div exact
