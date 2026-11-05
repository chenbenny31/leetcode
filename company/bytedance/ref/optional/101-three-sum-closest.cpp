// ByteDance / LC 16: return the sum of three integers in nums (n >= 3) closest to
// target; exactly one closest sum exists.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::abs;

// Sort, fix i, two pointers on the rest. T: O(n^2), S: O(1) besides the sort.
int threeSumClosest(vector<int> nums, int target) {
    sort(nums.begin(), nums.end());
    int n = nums.size();
    int best = nums[0] + nums[1] + nums[2];
    for (int i = 0; i < n - 2; i++) {
        int lo = i + 1;
        int hi = n - 1;
        while (lo < hi) {
            int sum = nums[i] + nums[lo] + nums[hi];
            if (abs(sum - target) < abs(best - target)) { best = sum; }
            if (sum == target) { return sum; }
            // sorted: too small -> raise the low end, too big -> drop the high end
            if (sum < target) {
                lo++;
            } else {
                hi--;
            }
        }
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (threeSumClosest(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {-1, 2, 1, -4}, 1, 2);
    test("all-zeros", {0, 0, 0}, 1, 0);
    test("negatives", {-3, -2, -5, 3, -4}, -1, -2);
}

// follow-up: zero-sum triplets (LC 15) -> same sort + two pointers; skip duplicate
//   nums[i], move lo/hi past equal values after a match, O(n^2)
// follow-up: count triplets with sum < target (LC 259) -> if sum < target:
//   count += hi - lo, lo++; else hi--, O(n^2)
// follow-up: 4Sum (LC 18) -> fix two indices + two pointers, O(n^3);
//   kSum by recursion, O(n^(k-1))
