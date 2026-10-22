// LC 1249 Minimum Remove to Make Valid Parentheses: given a string of lowercase letters
// and parentheses, remove the fewest parentheses so the result is valid.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// One pass matches ')' against a stack of open indices; whatever stays unmatched (extra
// ')' at once, leftover '(' at the end) is marked and then dropped. T: O(n), S: O(n).
string minRemoveToMakeValid(string s) {
    int n = s.size();
    vector<int> open;   // indices of '(' not matched yet
    for (int i = 0; i < n; i++) {
        if (s[i] == '(') {
            open.push_back(i);
        } else if (s[i] == ')') {
            if (open.empty()) {
                s[i] = '*';
            } else {
                open.pop_back();
            }
        }
    }
    for (int i : open) {
        s[i] = '*';
    }
    string result;
    for (char c : s) {
        if (c != '*') { result += c; }
    }
    return result;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (minRemoveToMakeValid(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("extra-close", "a)b(c)d", "ab(c)d");
    test("extra-open", "(a(b)", "a(b)");
    test("all-bad", "))((", "");
    test("already-valid", "(a)b", "(a)b");
}
