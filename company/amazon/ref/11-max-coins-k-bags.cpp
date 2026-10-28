// LC 3413: bags sit on a number line. coins[i] = [l, r, c] means every bag in [l, r]
// holds c coins; the segments do not overlap. Return the max coins in k consecutive bags.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;
using std::min;

// coins from segment seg inside the window [s, s + k - 1]
long long overlap(const vector<int>& seg, long long s, int k) {
    long long lo = max<long long>(seg[0], s);
    long long hi = min<long long>(seg[1], s + k - 1);
    if (lo > hi) { return 0; }
    return (hi - lo + 1) * seg[2];
}

// straightforward: an optimal window starts at some l or ends at some r; try each of
// those 2n windows against every segment. T: O(n^2), S: O(1).
long long maximumCoins(const vector<vector<int>>& coins, int k) {
    long long best = 0;
    for (const auto& seg : coins) {
        long long starts[2] = {seg[0], (long long)seg[1] - k + 1};
        for (long long s : starts) {
            long long total = 0;
            for (const auto& other : coins) { total += overlap(other, s, k); }
            best = max(best, total);
        }
    }
    return best;
}

// windows that start at each segment's l: two pointers over segments sorted by l.
// segs[j] is the first segment not fully inside the window; full = coins of segs[i..j).
long long bestStartAligned(vector<vector<int>> segs, int k) {
    sort(segs.begin(), segs.end());
    int n = segs.size();
    long long best = 0, full = 0;
    int j = 0;
    for (int i = 0; i < n; i++) {
        long long end = (long long)segs[i][0] + k - 1;
        while (j < n && segs[j][1] <= end) {
            full += (long long)(segs[j][1] - segs[j][0] + 1) * segs[j][2];
            j++;
        }
        long long part = 0;
        if (j < n && segs[j][0] <= end) { part = (end - segs[j][0] + 1) * segs[j][2]; }
        best = max(best, full + part);
        if (j > i) {
            full -= (long long)(segs[i][1] - segs[i][0] + 1) * segs[i][2];
        } else {
            j = i + 1;
        }
    }
    return best;
}

// improved: windows ending at some r are windows starting at some l after mirroring the
// line (x -> -x). T: O(n log n), S: O(n).
long long maximumCoinsWindow(const vector<vector<int>>& coins, int k) {
    vector<vector<int>> mirrored;
    for (const auto& seg : coins) { mirrored.push_back({-seg[1], -seg[0], seg[2]}); }
    return max(bestStartAligned(coins, k), bestStartAligned(mirrored, k));
}

void test(const string& label, const vector<vector<int>>& coins, int k, long long want) {
    bool ok = maximumCoins(coins, k) == want && maximumCoinsWindow(coins, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("span-gap", {{1, 3, 2}, {5, 6, 4}}, 4, 10);
    test("inside", {{1, 10, 3}}, 2, 6);
    test("k-big", {{1, 1, 5}, {3, 3, 7}}, 10, 12);
    test("touch", {{1, 2, 1}, {3, 4, 5}}, 2, 10);
    test("end-aligned", {{1, 1, 9}, {4, 6, 1}}, 3, 9);
}
