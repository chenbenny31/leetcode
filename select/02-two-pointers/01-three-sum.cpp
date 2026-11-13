// LC 15 3Sum (Medium) — sort + fix one + two pointers
// Return all unique triplets of values from nums (distinct indices) that sum to 0.
// Clarify: no duplicate triplets in the output; output order; fewer than 3 numbers ->
//   empty; the input may be sorted (a copy here).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Core idea: sort, fix the smallest value, and close two pointers on the rest; skip
// equal values at both levels so no triplet repeats. T: O(n^2), S: O(1) + output.
vector<vector<int>> threeSum(vector<int> nums) {
    sort(nums.begin(), nums.end());
    // triplets, each ascending, found in ascending order
    vector<vector<int>> result;
    int n = nums.size();
    for (int i = 0; i < n - 2; i++) {
        // same first value would give the same triplets
        if (i > 0 && nums[i] == nums[i - 1]) {
            continue;
        }
        int left = i + 1;
        int right = n - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum < 0) {
                left++;
            } else if (sum > 0) {
                right--;
            } else {
                result.push_back({nums[i], nums[left], nums[right]});
                left++;
                right--;
                // skip equal values so the same pair is not added again
                while (left < right && nums[left] == nums[left - 1]) {
                    left++;
                }
            }
        }
    }
    return result;
}

void test(const string& label, const vector<int>& nums,
          const vector<vector<int>>& want) {
    cout << label << (threeSum(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dup-values", {2, -1, 0, -1, 1}, {{-1, -1, 2}, {-1, 0, 1}});
    test("all-zeros", {0, 0, 0, 0}, {{0, 0, 0}});
    test("dup-pair", {-2, 1, 1, 1, 1}, {{-2, 1, 1}});
    test("none", {1, 2, -4}, {});
    return 0;
}

// follow-up (ideas only):
// - 3Sum closest (LC 16) -> same loops; keep the sum with the smallest |sum - target|,
//   move left up when the sum is below target, right down when above
// - count triplets with sum < target (LC 259) -> when the sum with left and right is
//   below target, every pair (left, left + 1..right) fits: add right - left, left++
// - 4Sum / kSum (LC 18) -> fix k - 2 values with duplicate skips, two pointers inside;
//   O(n^(k-1)), long long for the sum
