// LC 20 Valid Parentheses (Easy) — stack of open brackets
// s holds only ()[]{}; true if every bracket is closed by the same type in right order
//
// clarify:
// other chars -> assume none
// empty string -> valid
// len limit -> stack can hold all of s
// one bracket type only? -> follow-up, int depth

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// core idea: push every opener; a closer must match the top opener, then pop it
// valid iff the stack ends empty
// T: O(n), S: O(n)
bool isValid(const string& s) {
    // stk = unmatched open brackets, innermost at back
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

// follow-up:
// only '(' and ')' -> int depth instead of stack; fail if negative, valid if ends 0
// longest valid substring (LC 32) -> stack of idx seeded w/ -1; len = i - top after pop
