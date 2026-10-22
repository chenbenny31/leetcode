// LC 93 Restore IP Addresses: given a digit string s, return every valid IPv4 address
// formed by inserting three dots. Each part is 0-255 with no leading zeros.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

void dfs(const string& s, int start, vector<string>& parts, vector<string>& out) {
    int n = s.size();
    if (parts.size() == 4) {
        if (start == n) {
            out.push_back(parts[0] + "." + parts[1] + "." + parts[2] + "." + parts[3]);
        }
        return;
    }
    int value = 0;
    for (int end = start; end < n && end < start + 3; end++) {
        value = value * 10 + (s[end] - '0');
        if (value > 255) { break; }
        parts.push_back(s.substr(start, end - start + 1));
        dfs(s, end + 1, parts, out);
        parts.pop_back();
        // a leading zero allows only the one-digit part "0"
        if (s[start] == '0') { break; }
    }
}

// Backtracking: choose a 1-3 digit part four times and require all digits used.
// parts holds the chosen parts. T: O(3^4 * 4), i.e. constant, S: O(1).
vector<string> restoreIpAddresses(string s) {
    vector<string> out;
    vector<string> parts;
    dfs(s, 0, parts, out);
    return out;
}

void test(const string& label, const string& s, vector<string> want) {
    vector<string> got = restoreIpAddresses(s);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("leading-zero", "10203", {"1.0.20.3", "10.2.0.3"});
    test("all-zero", "0000", {"0.0.0.0"});
    test("too-short", "111", {});
    test("max-parts", "255255255255", {"255.255.255.255"});
}
