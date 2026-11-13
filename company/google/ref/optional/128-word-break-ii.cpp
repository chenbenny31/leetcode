// LC 140 Word Break II: string s and a dictionary of words (reusable any number of
// times). Return every way to split s into dictionary words, as sentences with words
// joined by single spaces, in any order; {} when no split exists.
//
// Clarify:
// - |s| <= 20 in LC; the output itself can be exponential in general
// - duplicates in the dictionary -> the set removes them
// Idea: dfs(start) = all sentences for s[start..]; memo by start. For every word
//   s[start..end), prepend it to each sentence of dfs(end). dfs(n) = {""}.
// Pitfalls:
// - base case returns {""} (one empty sentence), not {} (no sentence)
// - no space after the last word: add " " + tail only when tail is non-empty
// - dead suffixes are memoized as {}, so they are expanded only once
// Tests to write: two splits sharing a word; reuse of short words; no split;
//   whole string is one word.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::sort;

// memo[start] = all sentences for s[start..]
vector<string> dfs(const string& s, int start, const unordered_set<string>& dict,
                   unordered_map<int, vector<string>>& memo) {
    if (start == (int)s.size()) { return {""}; }
    if (memo.count(start)) { return memo[start]; }
    vector<string> result;
    for (int end = start + 1; end <= (int)s.size(); end++) {
        string word = s.substr(start, end - start);
        if (!dict.count(word)) { continue; }
        for (const string& tail : dfs(s, end, dict, memo)) {
            result.push_back(tail.empty() ? word : word + " " + tail);
        }
    }
    memo[start] = result;
    return result;
}

// memoized DFS over split points. T: O(n^2 + output size), S: O(n * output) memo.
vector<string> wordBreak(const string& s, const vector<string>& wordDict) {
    // dictionary words for O(1) lookup
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    unordered_map<int, vector<string>> memo;
    return dfs(s, 0, dict, memo);
}

void test(const string& label, const string& s, const vector<string>& wordDict,
          vector<string> want) {
    vector<string> got = wordBreak(s, wordDict);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-splits", "catsdog", {"cat", "cats", "sand", "dog", "s"},
         {"cats dog", "cat s dog"});
    test("reuse", "aaaa", {"a", "aa"},
         {"a a a a", "aa a a", "a aa a", "a a aa", "aa aa"});
    test("no-split", "abc", {"ab"}, {});
    test("one-word", "dog", {"dog"}, {"dog"});
}

// follow-up: only yes / no (LC 139) -> dp[i] = some j < i with dp[j] and s[j..i) in
//   dict. O(n^2)
// follow-up: count the sentences -> same DP with sums: dp[i] += dp[j] for each word
