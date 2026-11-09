// LC 139 Word Break (Medium). Pattern: prefix DP over split points.
// Can s be split into a sequence of words from wordDict? Words may be reused.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// s[0..i) splits if some word ends at i and the prefix before it splits.
// T: O(n * W * L) for W words of length <= L, S: O(n).
bool wordBreak(const string& s, const vector<string>& wordDict) {
    int n = s.size();
    // dp[i] = s[0..i) can be split into dictionary words
    vector<bool> dp(n + 1, false);
    dp[0] = true;
    for (int i = 1; i <= n; i++) {
        for (const string& word : wordDict) {
            int len = word.size();
            // word would occupy s[start..i)
            int start = i - len;
            if (start >= 0 && dp[start] && s.substr(start, len) == word) {
                dp[i] = true;
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
    test("greedy-trap", "abcd", {"a", "abc", "bcd"}, true);
    test("reuse", "aaaaa", {"aa", "aaa"}, true);
    test("no-split", "aaaa", {"aaa"}, false);
}

// follow-up: return all sentences (LC 140) -> dfs(start) memoized in
//   unordered_map<int, vector<string>>; output itself can be exponential
// follow-up: huge dict -> unordered_set of words, try s.substr(j, i - j) for j in
//   [i - maxLen, i), O(n * maxLen^2)
