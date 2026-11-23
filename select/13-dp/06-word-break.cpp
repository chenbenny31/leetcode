// LC 139 Word Break (Medium) — recursion + memo, or prefix DP over split points
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
#include <unordered_set>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;

// ==================== solution 1: recursion + memo (start here) ====================
// same shape as Word Break II: cut a dict word off the front, ask if the rest splits

// can s[start..] be split; memo[start] caches the answer for each suffix
bool canSplit(const string& s, int start, const unordered_set<string>& dict,
              unordered_map<int, bool>& memo) {
    int n = s.size();
    if (start == n) { return true; }
    if (memo.count(start)) { return memo[start]; }
    for (int end = start + 1; end <= n; end++) {
        if (dict.count(s.substr(start, end - start)) && canSplit(s, end, dict, memo)) {
            memo[start] = true;
            return true;
        }
    }
    memo[start] = false;
    return false;
}

// T: O(n^3) — n suffixes, n cuts each, O(n) substr, S: O(n + dict)
bool wordBreakMemo(const string& s, const vector<string>& wordDict) {
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    unordered_map<int, bool> memo;
    return canSplit(s, 0, dict, memo);
}

// ==================== solution 2: bottom-up DP ====================
// same idea filled left to right: s[0..i) splits if a word ends at i and the prefix
// before that word splits

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

// ==================== tests ====================

void test(const string& label, const string& s, const vector<string>& dict, bool want) {
    bool ok = wordBreak(s, dict) == want && wordBreakMemo(s, dict) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-words", "applepen", {"apple", "pen"}, true);
    test("greedy-trap", "abcd", {"a", "abc", "bcd"}, true);
    test("reuse", "aaaaa", {"aa", "aaa"}, true);
    test("no-split", "aaaa", {"aaa"}, false);
}

// follow-up:
// 1. return all sentences (LC 140) -> backtracking; memo dfs(start) -> its sentences
// 2. huge dict -> unordered_set; try s.substr(j, i - j) for j in [i - maxLen, i)
// 3. cnt element-symbol spellings (Google) -> lowercase; dp[i] += dp[start], long long
// 4. decode ways (LC 91) -> dp[i] += dp[i - 1] if s[i - 1] != '0', += dp[i - 2] if 10..26
// 5. Morse decodings cnt -> same counting DP, dp[i] += dp[i - len] per code ending at i
