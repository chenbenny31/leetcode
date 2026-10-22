// LC 140 Word Break II: given string s and wordDict, return every sentence made by
// inserting spaces so that each word is in the dictionary (any order of answers).
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using std::cout;
using std::sort;
using std::string;
using std::unordered_map;
using std::vector;

// Memoised DFS: all sentences for the suffix s[start..). T: O(n * W * L + output),
// S: O(output) for the memo (the output itself can be exponential).
vector<string> dfs(const string& s, int start, const vector<string>& wordDict,
                   unordered_map<int, vector<string>>& memo) {
    int n = s.size();
    if (start == n) {
        return {""};
    }
    auto it = memo.find(start);
    if (it != memo.end()) {
        return it->second;
    }
    vector<string> res;
    for (const auto& w : wordDict) {
        int len = w.size();
        if (start + len > n || s.compare(start, len, w) != 0) {
            continue;
        }
        for (const auto& rest : dfs(s, start + len, wordDict, memo)) {
            res.push_back(rest.empty() ? w : w + " " + rest);
        }
    }
    memo[start] = res;
    return res;
}

vector<string> wordBreak(const string& s, const vector<string>& wordDict) {
    // start -> sentences for s[start..), filled once per start
    unordered_map<int, vector<string>> memo;
    return dfs(s, 0, wordDict, memo);
}

void test(const string& label, const string& s, const vector<string>& dict,
          const vector<string>& want) {
    vector<string> got = wordBreak(s, dict);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-ways", "catsdog", {"cat", "cats", "dog", "sdog"},
         {"cat sdog", "cats dog"});
    test("reuse", "aaa", {"a", "aa"}, {"a a a", "a aa", "aa a"});
    test("none", "ab", {"a"}, {});
}
