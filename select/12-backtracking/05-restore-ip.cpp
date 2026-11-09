// LC 93 Restore IP Addresses (Medium). Pattern: backtracking over 4 parts of 1-3 digits.
// s holds only digits; return every valid IPv4 address made by inserting three dots.
// Each part is 0..255 with no leading zero ("0" itself is fine).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// address = dotted IP built from s[0..start); parts = parts placed so far
void dfs(const string& s, int start, int parts, string& address, vector<string>& result) {
    int n = s.size();
    if (parts == 4) {
        if (start == n) { result.push_back(address); }
        return;
    }
    // numeric value of the part s[start, start + len)
    int partValue = 0;
    for (int len = 1; len <= 3 && start + len <= n; len++) {
        char digit = s[start + len - 1];
        partValue = partValue * 10 + (digit - '0');
        if (partValue > 255) { return; }
        // "0x" / "0xx": leading zero
        if (len > 1 && s[start] == '0') { return; }
        int oldLen = address.size();
        if (parts > 0) { address += '.'; }
        address += s.substr(start, len);
        dfs(s, start + len, parts + 1, address, result);
        // undo this part
        address.resize(oldLen);
    }
}

// At most 3^4 branches, each copying an O(n) string. T: O(3^4 * n), S: O(n).
vector<string> restoreIpAddresses(const string& s) {
    vector<string> result;
    string address;
    dfs(s, 0, 0, address, result);
    return result;
}

void test(const string& label, const string& s, const vector<string>& want) {
    cout << label << (restoreIpAddresses(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("ones", "1111", {"1.1.1.1"});
    test("leading-zero", "010010", {"0.10.0.10", "0.100.1.0"});
    test("over-255", "256256256256", {});
    test("too-short", "123", {});
}

// follow-up: prune early -> return if the digits left are < 4 - parts or
//   > 3 * (4 - parts)
// follow-up: validate one IP (LC 468) -> char-loop split on '.', same per-part checks
