// ByteDance (FastPrep, phone, 2 sightings): LC 93 generalized. Insert segmentCount - 1
// dots into digits so every segment is 0..255 with no leading zero (except "0"). Return
// all results in lexicographic order. LC 93 is segmentCount = 4.

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

// Backtracking over segment lengths; shorter first and '.' < every digit, so results
// come out already sorted. T: O(3^k * n) for k segments, S: O(n) besides the output.
vector<string> restoreAddresses(const string& digits, int segmentCount) {
    // valid addresses in DFS (= lexicographic) order
    vector<string> result;
    // address built so far
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
    test("k1-too-big", "256", 1, {});
    test("too-long", "1234567", 2, {});
}

// follow-up: count only -> dp[i][j] = ways to split digits[0..i) into j segments, sum
//   over len 1..3 with a valid last segment; O(3 * n * k), long long
// follow-up: segment range [0, M] -> validSegment compares to M; max len = digits of M
// follow-up: memoize dead states -> dead[start][left] skips repeated failing subtrees;
//   the output itself can still be exponential
