// LC 131 Palindrome Partitioning (Medium). Pattern: backtracking on palindromic prefixes.
// Return every way to split s into substrings that are all palindromes.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

bool isPalindrome(const string& s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) { return false; }
        left++;
        right--;
    }
    return true;
}

// path = pieces chosen for s[0..start)
void dfs(const string& s, int start, vector<string>& path,
         vector<vector<string>>& result) {
    int n = s.size();
    if (start == n) {
        result.push_back(path);
        return;
    }
    for (int end = start; end < n; end++) {
        // only a palindromic piece can lead to a valid split
        if (!isPalindrome(s, start, end)) { continue; }
        path.push_back(s.substr(start, end - start + 1));
        dfs(s, end + 1, path, result);
        path.pop_back();
    }
}

// Choose the next palindromic prefix of the rest. T: O(n * 2^n), S: O(n) + output.
vector<vector<string>> partition(const string& s) {
    vector<vector<string>> result;
    vector<string> path;
    dfs(s, 0, path, result);
    return result;
}

void test(const string& label, const string& s, const vector<vector<string>>& want) {
    vector<vector<string>> got = partition(s);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", "aba", {{"a", "b", "a"}, {"aba"}});
    test("pairs", "aabb", {{"a", "a", "b", "b"}, {"a", "a", "bb"},
                           {"aa", "b", "b"}, {"aa", "bb"}});
    test("no-long", "ab", {{"a", "b"}});
}

// follow-up: O(1) palindrome checks -> precompute pal[i][j] = s[i] == s[j] &&
//   (j - i < 2 || pal[i + 1][j - 1]), O(n^2)
// follow-up: min cuts only (LC 132) -> dp over the same pal table, O(n^2)
