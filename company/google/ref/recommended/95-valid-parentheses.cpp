// 95 · Valid Parentheses — LC 20
// Mother: select/05-stack/01-valid-parentheses.cpp
// s holds only ()[]{}; true if every bracket is closed by the same type, in order
// reported: parentheses with deletions x2 (Optional table)
//
// clarify:
// empty string -> valid
// other chars -> none, only the six brackets
// closer w/ empty stack -> invalid

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// core idea: push openers; a closer must match the top; leftover openers -> invalid
// T: O(n), S: O(n)
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

// follow-up:
// only '(' and ')' -> int depth instead of a stack; fail when it goes negative
// longest valid substring (LC 32) -> stack of idx seeded w/ -1; on ')' pop
// LC 32 after the pop -> push i if the stack is empty, else len = i - new top
