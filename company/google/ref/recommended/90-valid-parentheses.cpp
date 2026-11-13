// 90 · Valid Parentheses — LC 20
// Mother problem: select/05-stack/01-valid-parentheses.cpp
//
// LC 20 (Easy), stack of open brackets: s holds only ()[]{}; true if every bracket is
// closed by the same type, in order.
// Google reports: parentheses with deletions ×2 (Optional table)
//
// Clarify
// - empty string → valid; each closer must match the same type, in order
//
// Core idea
// - Push openers; a closer must match the top, else false; leftover openers → false.
// - T O(n), S O(n).
//
// Tests: nested, crossed, unclosed, extra closer
//
// Follow-ups
// - only '(' and ')' → an int depth instead of a stack, fail if it goes negative, O(1)
// - longest valid substring (LC 32) → stack of indices seeded with -1; on ')' pop, if
//   empty push i, else best = max(best, i - stk.back())
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// Push openers; a closer must match the top. T: O(n), S: O(n).
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
}
