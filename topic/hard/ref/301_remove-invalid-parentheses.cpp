// LC 301 Remove Invalid Parentheses: remove the minimum number of parentheses from s
// (letters stay) to make it valid. Return all distinct valid strings, in any order.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::sort;

bool isValid(const string& s) {
    int open = 0;   // unmatched '(' so far
    for (char c : s) {
        if (c == '(') {
            open++;
        } else if (c == ')') {
            if (open == 0) { return false; }
            open--;
        }
    }
    return open == 0;
}

// BFS by number of removals: the first level that contains a valid string is minimal.
// T: O(2^n * n) worst case, S: O(2^n * n).
vector<string> removeInvalidParentheses(string s) {
    unordered_set<string> cur = {s};   // distinct strings with exactly `removed` removals
    while (true) {
        vector<string> valid;
        for (const auto& str : cur) {
            if (isValid(str)) { valid.push_back(str); }
        }
        if (!valid.empty()) { return valid; }
        unordered_set<string> nxt;
        for (const auto& str : cur) {
            for (int i = 0; i < (int)str.size(); i++) {
                if (str[i] != '(' && str[i] != ')') { continue; }
                nxt.insert(str.substr(0, i) + str.substr(i + 1));
            }
        }
        cur = nxt;
    }
}

void test(const string& label, const string& s, vector<string> want) {
    vector<string> got = removeInvalidParentheses(s);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-answers", "())", {"()"});
    test("duplicates-collapse", "()())", {"(())", "()()"});
    test("already-valid", "(a)", {"(a)"});
    test("all-removed", "))", {""});
}
