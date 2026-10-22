// LC 1494 Parallel Courses II: n courses, relations {a, b} = take a before b, at most k
// courses per semester. Return the minimum number of semesters to take all of them.
#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::min;

// Bitmask DP over sets of finished courses. From a set, the available courses are those
// not taken whose prereqs are all inside it; take any subset of them of size <= k.
// T: O(3^n), S: O(2^n).
int minNumberOfSemesters(int n, vector<vector<int>>& relations, int k) {
    vector<int> pre(n, 0);   // pre[i] = bitmask of the courses required before course i
    for (const auto& r : relations) { pre[r[1] - 1] |= 1 << (r[0] - 1); }
    int inf = INT_MAX / 2;
    // dp[mask] = fewest semesters to have finished exactly the courses in mask
    vector<int> dp(1 << n, inf);
    dp[0] = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        if (dp[mask] == inf) { continue; }
        int avail = 0;
        for (int i = 0; i < n; i++) {
            bool taken = (mask >> i) & 1;
            if (!taken && (pre[i] & mask) == pre[i]) { avail |= 1 << i; }
        }
        for (int sub = avail; sub > 0; sub = (sub - 1) & avail) {
            if (__builtin_popcount(sub) > k) { continue; }
            dp[mask | sub] = min(dp[mask | sub], dp[mask] + 1);
        }
    }
    return dp[(1 << n) - 1];
}

void test(const string& label, int n, vector<vector<int>> relations, int k, int want) {
    cout << label << (minNumberOfSemesters(n, relations, k) == want ? " PASS\n"
                                                                     : " FAIL\n");
}

int main() {
    test("independent", 3, {}, 2, 2);
    test("chain-with-fillers", 5, {{1, 2}, {2, 3}}, 2, 3);
    test("diamond", 4, {{1, 2}, {1, 3}, {2, 4}, {3, 4}}, 2, 3);
}
