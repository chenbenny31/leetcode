// Evaluate Nested Function-Call Expression (Google, interview report)
// evaluate e.g. "add(1, mul(2, sub(5, 3)))"; add, sub, mul, div, pow take exactly 2 args
// arg = int literal (maybe negative) or a nested call
//
// clarify:
// spaces around args -> allowed, skip them
// input well-formed? -> yes; malformed is a follow-up
// div / pow -> div truncates toward 0, divisor != 0; exponent >= 0
// values -> fit in long long
// nesting depth -> if too deep for recursion, solution 2

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

// move pos past spaces
void skipSpaces(const string& s, int& pos) {
    int n = s.size();
    while (pos < n && s[pos] == ' ') { pos++; }
}

// apply one function to its 2 values; pow by repeated multiplication
long long apply(const string& name, long long a, long long b) {
    if (name == "add") { return a + b; }
    if (name == "sub") { return a - b; }
    if (name == "mul") { return a * b; }
    if (name == "div") { return a / b; }
    long long result = 1;
    for (long long i = 0; i < b; i++) { result *= a; }
    return result;
}

// solution 1: recursive descent; the natural first answer
// core idea: arg := integer | name '(' arg ',' arg ')'; one function parses one arg
// all calls share pos by ref, each leaves it just past what it consumed
// T: O(n), S: O(depth)
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

// parse the whole string as one arg
long long evaluate(const string& s) {
    int pos = 0;
    return parseArg(s, pos);
}

// an open call: its name and the arg values parsed so far
struct Frame {
    string name;
    vector<long long> args;
};

// solution 2: explicit stack; use it when nesting is too deep for recursion
// core idea: a name opens a frame; a num or a ')' closing the top frame gives a value
// the value goes to the innermost open frame, or is the answer at top level
// T: O(n), S: O(depth)
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
    test("nested", "sub(mul(2, 3), add(1, 1))", 4);
    test("div-pow", "div( pow(2, 10) , 4 )", 256);
    test("long-long", "mul(3000000000,mul(2,-2))", -12000000000LL);
    test("div-truncate", "div(-7, 2)", -3);
}

// follow-up:
// variadic add(1, 2, 3) -> after 1st arg, loop while next is ',': acc = apply(acc, arg)
// malformed input -> check pos < n before every read; checked skips of '(' ',' ')'
// huge exponent -> square-and-multiply, O(log b) instead of O(b)
// add an operator -> one more line in apply; parser unchanged
// tokenizer first -> name / '(' / ')' / ',' / num tokens; same recursion over tokens
// LC 3749 Evaluate Valid Expressions -> this code as is
