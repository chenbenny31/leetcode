// seg overlaps? each bag at most one seg
// segs sorted? no
// any bags outside seg? 0
// range of l, r, k, amount: could need long long
// range of n: 1e5 -> O(n^2) slow
// window out of bags: 0
// return: only the max total

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

// const long long MOD = 1'000'000'007;

// prefix sum of all bags at <= x
long long prefixCoins(const vector<vector<int>>& segs, const vector<int>& starts,
                      const vector<long long>& prefix, long long x) {
    // segs 0..i-1 start at or before x
    int i = upper_bound(starts.begin(), starts.end(), x) - starts.begin();
    if (i == 0) { return 0; }
    // segs 0..i-2 fully left of x; seg i-1 need inspect
    auto& lastSeg = segs[i - 1];
    long long len = min<long long>(lastSeg[1], x) - lastSeg[0] + 1; // len of last possible overlap seg
    return prefix[i - 1] + len * lastSeg[2];

}

// optimal win starts at l or end at r, try 2n windows, T: O(nlogn), S: O(n)
long long maximumCoins(vector<vector<int>> coins, int k) {
    sort(coins.begin(), coins.end());
    int n = coins.size();
    vector<int> starts(n); // starts[i] = l of seg i, used for upper_bound search
    vector<long long> prefix(n + 1, 0); // prefix[i] = total coins in segs 0..i-1
    for (int i = 0; i < n; i++) {
        long long len = coins[i][1] - coins[i][0] + 1;
        starts[i] = coins[i][0];
        prefix[i + 1] = prefix[i] + len * coins[i][2];
    }

    long long best = 0;
    for (const auto& seg : coins) {
        long long l = seg[0];
        long long r = seg[1];
        // win[l, l + k - 1]
        long long fromL = prefixCoins(coins, starts, prefix, l + k - 1)
                        - prefixCoins(coins, starts, prefix, l - 1);
        // win[r - k + 1, r]
        long long toR = prefixCoins(coins, starts, prefix, r)
                      - prefixCoins(coins, starts, prefix, r - k);  
        best = max(best, max(fromL, toR));
    }
    // return best % MOD;
    return best;
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
