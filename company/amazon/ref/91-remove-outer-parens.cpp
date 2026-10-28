// LC 1021: s is a valid parentheses string. Split it into primitive parts and remove the
// outermost pair of each; return the result.

#include <iostream>
#include <string>

using std::cout;
using std::string;

// Track depth: an '(' at depth 0 or a ')' that returns to depth 0 is an outer one.
// T: O(n), S: O(n) output.
string removeOuterParentheses(const string& s) {
    string result;
    int depth = 0;
    for (char c : s) {
        if (c == '(') {
            if (depth > 0) { result += c; }
            depth++;
        } else {
            depth--;
            if (depth > 0) { result += c; }
        }
    }
    return result;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (removeOuterParentheses(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-parts", "(())()", "()");
    test("nested", "((()))", "(())");
    test("flat", "()()", "");
}
