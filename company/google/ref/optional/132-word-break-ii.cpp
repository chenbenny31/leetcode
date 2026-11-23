// 132 · Word Break II — LC 140
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

// ==================== solution 1: backtracking (start here) ====================
// cut a dict word off the front, recurse on the rest; at the end of s, the words
// picked on the way form one sentence

// words picked so far are in path; reaching the end of s = one full sentence
void backtrack(const string& s, int start, const unordered_set<string>& dict,
               vector<string>& path, vector<string>& out) {
    int n = s.size();
    if (start == n) {
        string sentence = path[0];
        for (int i = 1; i < (int)path.size(); i++) { sentence += " " + path[i]; }
        out.push_back(sentence);
        return;
    }
    for (int end = start + 1; end <= n; end++) {
        string word = s.substr(start, end - start);
        if (!dict.count(word)) { continue; }
        path.push_back(word);
        backtrack(s, end, dict, path, out);
        path.pop_back();
    }
}

// T: O(2^n * n) worst case (every cut works), S: O(n) besides the output
vector<string> wordBreak(const string& s, const vector<string>& wordDict) {
    // dictionary words for O(1) lookup
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    vector<string> path;
    vector<string> out;
    backtrack(s, 0, dict, path, out);
    return out;
}

// ==================== solution 2: memo (when asked to speed it up) ====================
// same cuts, but dfs(start) returns all sentences of s[start..] and is cached, so a
// suffix reached by several prefixes is solved once

// dfs(n) = {""}: one empty sentence, not "no sentence"
// T: O(n^2 + output size), S: O(n * output size) — memo; output can be exponential
vector<string> dfs(const string& s, int start, const unordered_set<string>& dict,
                   unordered_map<int, vector<string>>& memo) {
    if (start == (int)s.size()) { return {""}; }
    if (memo.count(start)) { return memo[start]; }
    vector<string> result;
    for (int end = start + 1; end <= (int)s.size(); end++) {
        string word = s.substr(start, end - start);
        if (!dict.count(word)) { continue; }
        // every sentence of the rest gets this word in front
        for (const string& tail : dfs(s, end, dict, memo)) {
            result.push_back(tail.empty() ? word : word + " " + tail);
        }
    }
    memo[start] = result;
    return result;
}

vector<string> wordBreakMemo(const string& s, const vector<string>& wordDict) {
    unordered_set<string> dict(wordDict.begin(), wordDict.end());
    // memo[start] = all sentences for s[start..]
    unordered_map<int, vector<string>> memo;
    return dfs(s, 0, dict, memo);
}

// ==================== tests ====================

void test(const string& label, const string& s, const vector<string>& wordDict,
          vector<string> want) {
    vector<string> got = wordBreak(s, wordDict);
    vector<string> gotMemo = wordBreakMemo(s, wordDict);
    sort(got.begin(), got.end());
    sort(gotMemo.begin(), gotMemo.end());
    sort(want.begin(), want.end());
    cout << label << (got == want && gotMemo == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-splits", "catsdog", {"cat", "cats", "sand", "dog", "s"},
         {"cats dog", "cat s dog"});
    test("reuse", "aaaa", {"a", "aa"},
         {"a a a a", "aa a a", "a aa a", "a a aa", "aa aa"});
    test("no-split", "abc", {"ab"}, {});
}

// follow-up:
// 1. only yes / no (LC 139) -> dp[i] = some j < i with dp[j] and s[j..i) in dict
// 2. count the sentences -> same DP with sums: dp[i] += dp[j] for each such word
// 3. many prefixes share a suffix -> solution 2, memo[start] = sentences of s[start..]
