// string s and dict of words, return every split of s into dict words
// join by single space in any order, {} if no split

// len os s -> output can be exp
// dup dict words -> need set to dedup
// any output order

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

// dsf(start) = all sentences for s[start..], memo by start
// for each dict word s[start..end) prepend in to each sentence of dfs(end)
// T: O(n^2), S: O(n) - memo
vector<string> dfs(const string& s, int start, const unordered_set<string>& dict,
                   unordered_map<int, vector<string>>& memo) {
    if (start == (int)s.size()) { return {""}; } // for tail.empty() ? 
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
    return result; // all valid sentence of s[start..]
}

vector<string> wordBreak(const string& s, const vector<string>& wordDict) {
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
