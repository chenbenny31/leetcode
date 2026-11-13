// LC 139 Word Break (Medium) — prefix DP over split points
// Can s be split into a sequence of words from wordDict? Words may be reused.
// Clarify: words reusable; every char of s must be covered; case-sensitive; empty s
//   is true; sizes of s and of the dictionary.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: the prefix s[0..i) splits if some word ends at i and the prefix before
// that word splits. T: O(n * W * L) for W words of length <= L, S: O(n).
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

// follow-up (ideas only):
// - return all sentences (LC 140) -> dfs(start) memoized in a map from start to its
//   list of sentences; the output itself can be exponential
// - huge dictionary -> unordered_set of words; try s.substr(j, i - j) for j in
//   [i - maxLen, i), O(n * maxLen^2)
// - count the ways to spell a word with element symbols, case-insensitive (Google) ->
//   lowercase both sides; dp[0] = 1 and dp[i] += dp[start] instead of OR; use long long
// - decode ways (LC 91) -> dp[i] += dp[i - 1] if s[i - 1] != '0', += dp[i - 2] if the
//   last two digits form 10..26
// - Morse decodings count -> the same counting DP, dp[i] += dp[i - len] for each code
//   that ends at i
