// 54 · 3Sum — LC 15, 259
// Mother problem: select/02-two-pointers/01-three-sum.cpp
//
// Google reports: sum below target ×2
//
// LC 15: return all unique triplets [a, b, c] of values from nums (distinct indices)
// with a + b + c == 0; no duplicate triplets.
//
// Clarify
// - values may repeat; each value triplet once
// - output order → each triplet ascending, triplets in ascending order
// - fewer than 3 numbers → empty result
//
// Core idea
// - Sort, fix the smallest value i, two pointers on the rest: sum < 0 → left++, > 0 →
//   right--.
// - Skip a repeated first value; after a hit, skip equal left values.
// - T O(n^2), S O(1) besides the output.
//
// Tests
// - duplicate values → two triplets; all zeros → one; {-2, 1, 1, 1, 1} → one; none
//
// Follow-ups
// - 3Sum closest (LC 16) → same loop with target, keep the sum with min |sum − target|
// - count triplets with sum < target (LC 259) → if first + left + right fits, all
//   right − left pairs with this left fit: count += right − left, left++; else right--
// - 4Sum / kSum (LC 18) → fix k − 2 values with dup skips, two pointers inside, long long

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Sort, fix the smallest value, two pointers on the rest. T: O(n^2), S: O(1) + output.
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
}
