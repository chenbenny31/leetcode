// ByteDance phone (FT): delete exactly one contiguous block of k elements from nums;
// the remaining elements are re-indexed from 0. Return the smallest start index of a
// block whose deletion makes the sum at even indices equal the sum at odd indices, or
// -1 if there is none. Key point: when k is odd, every element after the block shifts
// by an odd amount, so its index parity flips.
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

// Improved: prefix sums split by original index parity. For a block [s, s + k):
// - the prefix [0, s) keeps its indices;
// - the suffix [s + k, n) moves left by k, so its parity flips exactly when k is odd.
// Each start is then checked in O(1). T: O(n), S: O(n).
int firstBalancedBlock(const vector<int>& nums, int k) {
    int n = nums.size();
    // pe[i] / po[i] = sum of nums[j] for j < i with j even / odd
    vector<long long> pe(n + 1, 0);
    vector<long long> po(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pe[i + 1] = pe[i];
        po[i + 1] = po[i];
        if (i % 2 == 0) {
            pe[i + 1] += nums[i];
        } else {
            po[i + 1] += nums[i];
        }
    }

    for (int s = 0; s + k <= n; s++) {
        int e = s + k;
        long long sufEven = pe[n] - pe[e];
        long long sufOdd = po[n] - po[e];
        long long even = pe[s];
        long long odd = po[s];
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
