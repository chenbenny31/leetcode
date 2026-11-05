// ByteDance / LC 93: s holds only digits; return every valid IPv4 address formed by
// inserting three dots. Each part is 0..255 with no leading zero (except "0" itself).
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// try a 1-3 digit part at s[start..]; parts = segments placed so far
void dfs(const string& s, int start, int parts, string& cur, vector<string>& result) {
    int n = s.size();
    if (parts == 4) {
        if (start == n) { result.push_back(cur); }
        return;
    }
    int val = 0;
    for (int len = 1; len <= 3 && start + len <= n; len++) {
        val = val * 10 + (s[start + len - 1] - '0');
        if (val > 255) { return; }
        // "0x" / "0xx": leading zero
        if (len > 1 && s[start] == '0') { return; }
        int oldLen = cur.size();
        if (parts > 0) { cur += '.'; }
        cur += s.substr(start, len);
        dfs(s, start + len, parts + 1, cur, result);
        // undo this part
        cur.resize(oldLen);
    }
}

// Backtracking over 4 parts of 1-3 digits. T: O(3^4 * n), S: O(n).
vector<string> restoreIpAddresses(const string& s) {
    // valid addresses in DFS order
    vector<string> result;
    // address built so far
    string cur;
    dfs(s, 0, 0, cur, result);
    return result;
}

void test(const string& label, const string& s, const vector<string>& want) {
    cout << label << (restoreIpAddresses(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-ways", "25525511135", {"255.255.11.135", "255.255.111.35"});
    test("zeros", "0000", {"0.0.0.0"});
    test("inner-zero", "101023",
         {"1.0.10.23", "1.0.102.3", "10.1.0.23", "10.10.2.3", "101.0.2.3"});
}

// follow-up: prune early -> return if remaining digits < 4 - parts or > 3 * (4 - parts)
// follow-up: k segments instead of 4 -> parts limit k, same prune; O(3^k * n)
// follow-up: count only -> dp[i][p] = ways to split s[i..] into p valid parts, O(12n)
// follow-up: validate an IP (LC 468) -> char-loop split; v4: 4 parts, 1-3 digits, <= 255,
//   no leading 0; v6: 8 groups of 1-4 hex digits
