// ByteDance phone (FT): delete exactly one contiguous block of k elements from nums;
// the rest is re-indexed from 0. Return the smallest start of a block whose deletion
// makes the even-index sum equal the odd-index sum, or -1.
//   firstBalancedBlock(nums, k) -> int
// Constraints: n <= 1e5, 1 <= k <= n, |nums[i]| <= 1e9 (use 64-bit sums).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Straightforward: build the remaining array for every start and add it up.
// T: O(n^2), S: O(n).
int firstBalancedBlockBrute(const vector<int>& nums, int k) {
    int n = nums.size();
    for (int s = 0; s + k <= n; s++) {
        long long even = 0;
        long long odd = 0;
        // index of nums[i] after the deletion
        int idx = 0;
        for (int i = 0; i < n; i++) {
            if (i >= s && i < s + k) {
                continue;
            }
            if (idx % 2 == 0) {
                even += nums[i];
            } else {
                odd += nums[i];
            }
            idx++;
        }
        if (even == odd) {
            return s;
        }
    }
    return -1;
}

// Improved: prefix sums split by index parity; the suffix after the block shifts by k,
// so its parity flips iff k is odd. O(1) per start. T: O(n), S: O(n).
int firstBalancedBlock(const vector<int>& nums, int k) {
    int n = nums.size();
    // prefixEven[i] / prefixOdd[i] = sum of nums[j] for j < i with j even / odd
    vector<long long> prefixEven(n + 1, 0);
    vector<long long> prefixOdd(n + 1, 0);
    for (int i = 0; i < n; i++) {
        prefixEven[i + 1] = prefixEven[i];
        prefixOdd[i + 1] = prefixOdd[i];
        if (i % 2 == 0) {
            prefixEven[i + 1] += nums[i];
        } else {
            prefixOdd[i + 1] += nums[i];
        }
    }

    for (int s = 0; s + k <= n; s++) {
        int e = s + k;
        long long sufEven = prefixEven[n] - prefixEven[e];
        long long sufOdd = prefixOdd[n] - prefixOdd[e];
        long long even = prefixEven[s];
        long long odd = prefixOdd[s];
        // odd k: suffix even indices become odd and vice versa
        if (k % 2 == 0) {
            even += sufEven;
            odd += sufOdd;
        } else {
            even += sufOdd;
            odd += sufEven;
        }
        if (even == odd) {
            return s;
        }
    }
    return -1;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    bool ok = firstBalancedBlockBrute(nums, k) == want;
    ok = ok && firstBalancedBlock(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd-k-flips", {1, 5, 3, 2}, 1, 1);
    test("even-k", {4, 9, 9, 4}, 2, 1);
    test("delete-all", {7, 3}, 2, 0);
    test("delete-head", {9, 1, 1}, 1, 0);
    test("none", {1, 2}, 1, -1);
}

// follow-up: faster than O(n^2) -> see firstBalancedBlock() above; parity prefix sums,
//   suffix parity flips iff k odd, O(1) per start
// follow-up: delete one element, count ways (LC 1664) -> k = 1, count starts instead of
//   returning the first, O(n)
// follow-up: O(1) extra space -> totals + two running parity prefixes at s and s + k,
//   advance both as s moves
