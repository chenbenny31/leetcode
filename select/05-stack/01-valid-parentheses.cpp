// LC 20 Valid Parentheses (Easy). Pattern: stack of open brackets.
// s holds only ()[]{}. True if every bracket is closed by the same type, in order.

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

// follow-up: only '(' ')' -> int depth instead of a stack, fail if it goes negative, O(1)
// follow-up: longest valid substring (LC 32) -> stack of indices seeded with -1; on ')'
//   pop, if empty push i else best = max(best, i - stk.back())
