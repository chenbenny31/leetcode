// LC 139 Word Break: can string s be split into a sequence of words from wordDict
// (words may be reused)?
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Prefix dp: s[0..i) is breakable if it ends with a word whose start is breakable.
// T: O(n * W * L) for W words of length up to L, S: O(n).
bool wordBreak(const string& s, const vector<string>& wordDict) {
    int n = s.size();
    // dp[i] = 1 if s[0..i) can be split into dictionary words
    vector<int> dp(n + 1, 0);
    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (const auto& w : wordDict) {
            int len = w.size();
            if (len <= i && dp[i - len] && s.compare(i - len, len, w) == 0) {
                dp[i] = 1;
                break;
            }
        }
    }
    return dp[n];
}

void test(const string& label, const string& s, const vector<string>& dict, bool want) {
    cout << label << (wordBreak(s, dict) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-words", "applepen", {"apple", "pen"}, true);
    test("reuse", "aaaaa", {"aa", "aaa"}, true);
    test("no-split", "aaaa", {"aaa"}, false);
}
