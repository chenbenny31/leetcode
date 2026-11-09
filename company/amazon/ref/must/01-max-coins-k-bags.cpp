// Amazon "Find Maximum Total Amount" = LC 3413: bags sit on a number line.
// coins[i] = [l, r, c] means every bag in [l, r] holds c coins; the segments do not
// overlap. Return the max coins in k consecutive bags, modulo 1e9 + 7 (take the max
// first, then the mod; LC 3413 itself has no mod).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::upper_bound;
using std::max;
using std::min;

// prefix sum over positions: total coins in all bags at positions <= x
long long prefixCoins(const vector<vector<int>>& segs, const vector<int>& segStart,
                      const vector<long long>& segPrefix, long long x) {
    // segments 0..started-1 start at or before x
    int started = upper_bound(segStart.begin(), segStart.end(), x) - segStart.begin();
    if (started == 0) { return 0; }
    // all but the last of them lie fully left of x; only the last may be cut at x
    const auto& last = segs[started - 1];
    long long len = min<long long>(last[1], x) - last[0] + 1;
    return segPrefix[started - 1] + len * last[2];
}

// An optimal window starts at some l or ends at some r: try those 2n windows.
// Coins in window [s, e] = prefixCoins(e) - prefixCoins(s - 1). T: O(n log n), S: O(n).
long long maximumCoins(vector<vector<int>> coins, int k) {
    sort(coins.begin(), coins.end());
    int n = coins.size();
    // segStart[i] = l of segment i (sorted, for binary search)
    vector<int> segStart(n);
    // segPrefix[i] = total coins in segments 0..i-1
    vector<long long> segPrefix(n + 1, 0);
    for (int i = 0; i < n; i++) {
        long long len = coins[i][1] - coins[i][0] + 1;
        segStart[i] = coins[i][0];
        segPrefix[i + 1] = segPrefix[i] + len * coins[i][2];
    }

    long long best = 0;
    for (const auto& seg : coins) {
        long long l = seg[0];
        long long r = seg[1];
        // window [l, l + k - 1]
        long long fromL = prefixCoins(coins, segStart, segPrefix, l + k - 1)
                        - prefixCoins(coins, segStart, segPrefix, l - 1);
        // window [r - k + 1, r]
        long long toR = prefixCoins(coins, segStart, segPrefix, r)
                      - prefixCoins(coins, segStart, segPrefix, r - k);
        best = max(best, max(fromL, toR));
    }
    long long mod = 1000000007;
    return best % mod;
}

void test(const string& label, const vector<vector<int>>& coins, int k, long long want) {
    cout << label << (maximumCoins(coins, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("gap-unsorted", {{5, 6, 4}, {1, 3, 2}}, 4, 10);
    test("inside", {{1, 10, 3}}, 2, 6);
    test("k-big", {{1, 1, 5}, {3, 3, 7}}, 10, 12);
    test("end-only", {{1, 3, 1}, {4, 4, 9}}, 2, 10);
}
