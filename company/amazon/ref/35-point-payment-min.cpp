// programs[i] = {available points, cents redeemed per point}. Use a whole number of
// points from each program (within its balance) so that the redeemed total is exactly
// target cents. Return the min total number of points, or -1 if impossible.

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// straightforward: bounded knapsack; for each program try every count at every amount.
// T: O(target * total points), S: O(target).
int minPoints(const vector<vector<int>>& programs, int target) {
    // best[a] = min points that redeem exactly a cents
    vector<int> best(target + 1, INT_MAX);
    best[0] = 0;
    for (const auto& prog : programs) {
        int count = prog[0], value = prog[1];
        vector<int> next = best;
        for (int a = 0; a <= target; a++) {
            if (best[a] == INT_MAX) { continue; }
            for (int c = 1; c <= count && a + c * value <= target; c++) {
                next[a + c * value] = min(next[a + c * value], best[a] + c);
            }
        }
        best = next;
    }
    if (best[target] == INT_MAX) { return -1; }
    return best[target];
}

// improved: split each balance into bundles 1, 2, 4, ..., rest; every count 0..balance is
// a sum of distinct bundles, so 0/1 knapsack over bundles is enough.
// T: O(target * sum of log(points)), S: O(target).
int minPointsBinary(const vector<vector<int>>& programs, int target) {
    vector<int> best(target + 1, INT_MAX);
    best[0] = 0;
    for (const auto& prog : programs) {
        int left = prog[0], value = prog[1];
        for (int size = 1; left > 0; size *= 2) {
            int take = min(size, left);
            left -= take;
            long long cents = (long long)take * value;
            for (int a = target; a >= cents; a--) {
                if (best[a - cents] == INT_MAX) { continue; }
                best[a] = min(best[a], best[a - cents] + take);
            }
        }
    }
    if (best[target] == INT_MAX) { return -1; }
    return best[target];
}

void test(const string& label, const vector<vector<int>>& programs, int target,
          int want) {
    bool ok = minPoints(programs, target) == want &&
              minPointsBinary(programs, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mix", {{2, 5}, {10, 1}}, 12, 4);
    test("balance-limit", {{1, 5}, {10, 1}}, 12, 8);
    test("impossible", {{1, 5}}, 3, -1);
    test("zero", {{1, 5}}, 0, 0);
    test("big-value", {{3, 4}, {3, 6}}, 12, 2);
}
