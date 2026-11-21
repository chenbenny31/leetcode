// 06 · Min-Change Palindrome, Smallest (phone screen, Easy) — LC 2697 + the count
// return (min replacements to make s a palindrome, the lexicographically smallest
// palindrome reachable w/ that many replacements)
//
// clarify:
// only replacements -> yes, no insert / delete
// chars -> lowercase letters
// odd len -> the middle char never changes
// empty string -> not given (len >= 1)

#include <iostream>
#include <string>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::pair;
using std::min;

// core idea: each mismatched mirror pair costs exactly one change; make both the smaller
// char, which is also the lexicographically smallest choice
// T: O(n), S: O(n) — the copy
pair<int, string> makeSmallestPalindrome(string s) {
    int ops = 0;
    int i = 0;
    int j = (int)s.size() - 1;
    while (i < j) {
        if (s[i] != s[j]) {
            ops++;
            char c = min(s[i], s[j]);
            s[i] = c;
            s[j] = c;
        }
        i++;
        j--;
    }
    return {ops, s};
}

void test(const string& label, const string& s, int wantOps, const string& want) {
    pair<int, string> got = makeSmallestPalindrome(s);
    bool ok = got.first == wantOps && got.second == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-pair", "egcfe", 1, "efcfe");
    test("two-pairs", "abcd", 2, "abba");
    test("odd-middle", "seven", 1, "neven");
    test("already", "a", 0, "a");
    return 0;
}

// follow-up:
// smallest palindrome w/ at most k changes, k > mismatches -> spend leftover changes
//   from the outside in: a non-'a' pair -> "aa" costs 2 (1 if it was a mismatch already
//   paid), the middle char -> 'a' costs 1
// insertions instead of replacements (LC 1312) -> n - longest palindromic subsequence
// valid palindrome after deleting at most one char (LC 680) -> two pointers, on the
//   first mismatch try skipping either side
