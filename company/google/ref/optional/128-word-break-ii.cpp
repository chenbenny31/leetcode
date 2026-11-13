// 128 · Word Break II — LC 140
// string s and a dict of words (reusable); return every split of s into dict words
// sentences joined by single spaces, any order; {} if no split
//
// clarify:
// len of s -> output can be exponential
// dup dict words -> set dedups
// output order -> any

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

// core idea: dfs(start) = all sentences for s[start..], memo by start; dfs(n) = {""}
// for each dict word s[start..end) prepend it to each sentence of dfs(end)
// T: O(n^2 + output size), S: O(n * output size) — memo; output can be exponential
vector<string> dfs(const string& s, int start, const unordered_set<string>& dict,
                   unordered_map<int, vector<string>>& memo) {
    // one empty sentence, not "no sentence"
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

vector<string> wordBreak(const string& s, const vector<string>& wordDict) {
    // dictionary words for O(1) lookup
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    // memo[start] = all sentences for s[start..]
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

// follow-up:
// only yes / no (LC 139) -> dp[i] = some j < i w/ dp[j] and s[j..i) in dict, O(n^2)
// cnt the sentences -> same DP w/ sums: dp[i] += dp[j] for each such word
