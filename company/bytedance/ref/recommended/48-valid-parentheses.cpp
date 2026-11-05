// ByteDance (hack2hire) / LC 20: s holds only '(', ')', '[', ']', '{', '}'; return true
// if every bracket is closed by the same type in the correct order.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Stack of open brackets; a closer must match the top. T: O(n), S: O(n).
bool isValid(const string& s) {
    // unmatched open brackets, innermost at the back
    vector<char> stack;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            stack.push_back(c);
            continue;
        }
        char open = '{';
        if (c == ')') {
            open = '(';
        } else if (c == ']') {
            open = '[';
        }
        if (stack.empty() || stack.back() != open) {
            return false;
        }
        stack.pop_back();
    }
    // leftover openers were never closed
    return stack.empty();
}

void test(const string& label, const string& s, bool want) {
    cout << label << (isValid(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("nested", "{[()]}", true);
    test("wrong-order", "([)]", false);
    test("unclosed", "((", false);
    test("extra-close", "())", false);
}

// follow-up: only '(' ')' -> int depth instead of stack, fail if depth < 0, O(1) space
// follow-up: longest valid substring (LC 32) -> stack of indices seeded with -1; on ')'
//   pop, if empty push i else best = max(best, i - top), O(n)
// follow-up: min removals to make valid (LC 1249) -> stack of '(' indices, mark
//   unmatched ')' and leftover '(' for removal, O(n)
