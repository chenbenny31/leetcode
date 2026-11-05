// ByteDance / LC 316: remove duplicate letters from s (lowercase) so every letter
// appears once; return the lexicographically smallest such subsequence.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Monotonic stack; drop a larger top if it appears again later. T: O(n), S: O(1).
string removeDuplicateLetters(const string& s) {
    int n = s.size();
    // last[c] = last index of letter c in s
    vector<int> last(26, 0);
    for (int i = 0; i < n; i++) { last[s[i] - 'a'] = i; }
    // seen[c] = letter c is in stk
    vector<bool> seen(26, false);
    // result so far, as small as possible
    string stk;
    for (int i = 0; i < n; i++) {
        char c = s[i];
        if (seen[c - 'a']) { continue; }
        // a larger top that reappears later can be re-added after c
        while (!stk.empty() && stk.back() > c && last[stk.back() - 'a'] > i) {
            seen[stk.back() - 'a'] = false;
            stk.pop_back();
        }
        stk += c;
        seen[c - 'a'] = true;
    }
    return stk;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (removeDuplicateLetters(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", "bcabc", "abc");
    test("lc-ex2", "cbacdcbc", "acdb");
    test("no-repeats", "cba", "cba");
}

// follow-up: LC 1081 (smallest subsequence of distinct chars) -> identical code
// follow-up: lexicographically largest -> pop while stk.back() < c and it reappears later
// follow-up: remove k digits (LC 402) -> same stack, pop while top > c and k > 0
// follow-up: why O(1) space -> last/seen are size 26 and stk holds <= 26 letters
