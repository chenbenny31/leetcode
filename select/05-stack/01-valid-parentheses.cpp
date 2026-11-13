// LC 20 Valid Parentheses (Easy) — stack of open brackets
// s holds only ()[]{}. Return true if every bracket is closed by the same type, in the
// right order.
// Clarify: other characters possible; empty string (valid); length limit.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// Core idea: push every opener; a closer must match the opener on top, which is then
// popped; valid when the stack ends empty. T: O(n), S: O(n).
bool isValid(const string& s) {
    // unmatched open brackets, innermost at the back
    vector<char> stk;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            stk.push_back(c);
            continue;
        }
        char open = '{';
        if (c == ')') {
            open = '(';
        } else if (c == ']') {
            open = '[';
        }
        if (stk.empty() || stk.back() != open) {
            return false;
        }
        stk.pop_back();
    }
    // leftover openers were never closed
    return stk.empty();
}

void test(const string& label, const string& s, bool want) {
    cout << label << (isValid(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("nested", "{[()]}", true);
    test("crossed", "{(})", false);
    test("unclosed", "((", false);
    test("extra-close", "())", false);
    return 0;
}

// follow-up (ideas only):
// - only '(' and ')' -> an int depth instead of a stack; fail when it goes negative,
//   valid when it ends at 0, O(1) space
// - longest valid substring (LC 32) -> stack of indices seeded with -1; on ')' pop, then
//   push i if the stack is empty, else the length is i minus the new top
