// LC 131 Palindrome Partitioning: split s into substrings that are all palindromes.
// Return every such partition.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

bool isPalindrome(const string& s, int l, int r) {
    while (l < r) {
        if (s[l] != s[r]) { return false; }
        l++;
        r--;
    }
    return true;
}

void dfs(const string& s, int start, vector<string>& cur, vector<vector<string>>& out) {
    int n = s.size();
    if (start == n) {
        out.push_back(cur);
        return;
    }
    for (int end = start; end < n; end++) {
        // only extend with a palindromic piece; others cannot lead to a valid split
        if (!isPalindrome(s, start, end)) { continue; }
        cur.push_back(s.substr(start, end - start + 1));
        dfs(s, end + 1, cur, out);
        cur.pop_back();
    }
}

// Backtracking: choose the next palindromic prefix of the rest. cur holds the pieces
// chosen so far. T: O(n * 2^n), S: O(n) beyond the output.
vector<vector<string>> partition(string s) {
    vector<vector<string>> out;
    vector<string> cur;
    dfs(s, 0, cur, out);
    return out;
}

void test(const string& label, const string& s, vector<vector<string>> want) {
    vector<vector<string>> got = partition(s);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", "aba", {{"a", "b", "a"}, {"aba"}});
    test("no-long-palindrome", "ab", {{"a", "b"}});
    test("single", "z", {{"z"}});
}
