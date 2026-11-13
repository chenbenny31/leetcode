// LC 139 Word Break (Medium) — prefix DP over split points
// return if s splits into a sequence of words from wordDict; words reusable
//
// clarify:
// words reusable -> no used-tracking
// every char of s must be covered -> answer = dp[n]
// case-sensitive -> compare as is
// empty s -> true
// sizes of s / dict -> huge dict: hash set + maxLen (follow-up)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: s[0..i) splits if some word ends at i and the prefix before it splits
// T: O(n * W * L), S: O(n) — W words of len <= L
bool wordBreak(const string& s, const vector<string>& wordDict) {
    int n = s.size();
    // dp[i] = s[0..i) can be split into dict words
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

// follow-up:
// return all sentences (LC 140) -> memo dfs(start) -> its sentences; output exponential
// huge dict -> unordered_set; try s.substr(j, i - j) for j in [i - maxLen, i)
// cnt spellings w/ element symbols (Google) -> lowercase; dp[i] += dp[start], long long
// decode ways (LC 91) -> dp[i] += dp[i - 1] if s[i - 1] != '0', += dp[i - 2] if 10..26
// Morse decodings cnt -> same counting DP, dp[i] += dp[i - len] per code ending at i
