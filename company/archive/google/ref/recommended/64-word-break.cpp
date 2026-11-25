// 64 · Word Break — LC 139, 91
// Mother: select/13-dp/06-word-break.cpp
// can s be split into a sequence of words from wordDict?
// reported: LC 139 ×2, element-symbol spellings ×2, Morse decodings
//
// clarify:
// words reusable -> yes
// empty s -> true
// case-sensitive -> yes
// dict size, word len -> loop over words vs over substrings

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: dp[i] = s[0..i) splits; true if some word ends at i and dp[i - len] is true
// T: O(n * W * L), S: O(n) — W words of len <= L
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

// follow-up:
// cnt element-symbol spellings ("Bacon") -> dp[i] += dp[start] instead of OR; lowercase
// decode ways (LC 91) -> dp[i] += dp[i - 1] if s[i - 1] != '0', += dp[i - 2] if 10..26
// Morse decodings -> same counting DP over the codes ending at i
// all sentences (LC 140) -> memoized dfs(start); output can be exponential
// huge dict -> unordered_set, try s.substr(j, i - j) for j in [i - maxLen, i)
