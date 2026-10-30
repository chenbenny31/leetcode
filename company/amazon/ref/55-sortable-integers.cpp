// LC 3886: k is sortable if k divides n and nums can be sorted (non-decreasing) by
// cutting it into consecutive blocks of length k and rotating each block independently
// any number of times. Return the sum of all sortable k.

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// A block can be rotated into order iff, read cyclically, it has at most one descent.
// Then the blocks must also fit together: each block's max <= the next block's min.
// Check every divisor. T: O(n * d(n)) for d(n) divisors, S: O(1).
long long sortableSum(const vector<int>& nums) {
    int n = nums.size();
    long long total = 0;
    for (int k = 1; k <= n; k++) {
        if (n % k != 0) { continue; }
        bool ok = true;
        int prevMax = INT_MIN;
        for (int start = 0; start < n && ok; start += k) {
            int descents = 0, lo = INT_MAX, hi = INT_MIN;
            for (int i = start; i < start + k; i++) {
                int nxt = i + 1 < start + k ? i + 1 : start;
                if (nums[i] > nums[nxt]) { descents++; }
                lo = min(lo, nums[i]);
                hi = max(hi, nums[i]);
            }
            if (descents > 1 || lo < prevMax) { ok = false; }
            prevMax = hi;
        }
        if (ok) { total += k; }
    }
    return total;
}

void test(const string& label, const vector<int>& nums, long long want) {
    cout << label << (sortableSum(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("swap-two", {2, 1}, 2);
    test("sorted", {1, 2, 3, 4}, 7);
    test("pairs", {2, 1, 4, 3}, 2);
    test("equal", {5, 5}, 3);
    test("none", {3, 1, 2, 1}, 0);
}
