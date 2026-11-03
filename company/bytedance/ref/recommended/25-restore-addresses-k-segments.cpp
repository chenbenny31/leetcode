// ByteDance (FastPrep, phone, 2 sightings): LC 93 Restore IP Addresses with the
// follow-up "any number of segments". Given a digit string digits and segmentCount,
// insert exactly segmentCount - 1 dots so that every segment has 1-3 digits, a value in
// [0, 255], and no leading zero (except "0" itself). Return every valid result in
// lexicographic order, or an empty list. LC 93 is segmentCount = 4.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

bool validSegment(const string& s) {
    if (s.size() > 1 && s[0] == '0') {
        return false;
    }
    int value = 0;
    for (char ch : s) {
        value = value * 10 + (ch - '0');
    }
    return value <= 255;
}

// Try segment lengths 1, 2, 3 at start; left = segments still to place.
void dfs(const string& digits, int start, int left, string& cur, vector<string>& result) {
    int remaining = digits.size() - start;
    // each remaining segment needs 1 to 3 digits
    if (remaining < left || remaining > 3 * left) {
        return;
    }
    if (left == 0) {
        result.push_back(cur);
        return;
    }

    for (int len = 1; len <= 3 && len <= remaining; len++) {
        string segment = digits.substr(start, len);
        if (!validSegment(segment)) {
            continue;
        }
        int before = cur.size();
        if (!cur.empty()) {
            cur += ".";
        }
        cur += segment;
        dfs(digits, start + len, left - 1, cur, result);
        cur.resize(before);
    }
}

// Backtracking over segment lengths. Shorter lengths are tried first, and '.' sorts
// before every digit, so results already come out in lexicographic order.
// T: O(3^k * n) for k segments (n <= 3k), S: O(n) recursion plus output.
vector<string> restoreAddresses(const string& digits, int segmentCount) {
    vector<string> result;
    string cur;
    dfs(digits, 0, segmentCount, cur, result);
    return result;
}

void test(const string& label, const string& digits, int k, const vector<string>& want) {
    cout << label << (restoreAddresses(digits, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("k3-order", "1111", 3, {"1.1.11", "1.11.1", "11.1.1"});
    test("zeros", "000", 3, {"0.0.0"});
    test("leading-zero", "0011", 2, {});
    test("k1-valid", "255", 1, {"255"});
    test("k1-too-big", "256", 1, {});
    test("too-long", "1234567", 2, {});
}
