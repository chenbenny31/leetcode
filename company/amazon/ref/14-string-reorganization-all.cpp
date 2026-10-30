// Return every distinct rearrangement of s (lowercase letters) in which no two adjacent
// characters are equal, in any order; empty if there is none. (LC 767 asks for one.)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// build left to right from letter counts, never repeating the previous letter; using
// counts instead of positions avoids duplicate strings
void dfs(int cnt[], string& cur, int n, vector<string>& result) {
    if ((int)cur.size() == n) {
        result.push_back(cur);
        return;
    }
    for (int c = 0; c < 26; c++) {
        if (cnt[c] == 0) { continue; }
        if (!cur.empty() && cur.back() == 'a' + c) { continue; }
        cnt[c]--;
        cur.push_back('a' + c);
        dfs(cnt, cur, n, result);
        cur.pop_back();
        cnt[c]++;
    }
}

// backtracking over counts. T: O(26 * answers * n) roughly, S: O(n) besides output.
vector<string> reorganizeAll(const string& s) {
    // cnt[c] = letters c not placed yet
    int cnt[26] = {};
    for (char c : s) { cnt[c - 'a']++; }
    vector<string> result;
    string cur;
    dfs(cnt, cur, s.size(), result);
    return result;
}

void test(const string& label, const string& s, vector<string> want) {
    vector<string> got = reorganizeAll(s);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-way", "aab", {"aba"});
    test("impossible", "aaab", {});
    test("pairs", "aabb", {"abab", "baba"});
    test("distinct", "abc", {"abc", "acb", "bac", "bca", "cab", "cba"});
    test("single", "a", {"a"});
}
