// LC 15 3Sum (Medium) — sort + fix one + two pointers
// return all unique triplets of values (distinct idx) that sum to 0
//
// clarify:
// dup triplets in output -> not allowed, skip equal values
// output order -> any
// fewer than 3 nums -> empty
// may modify input? -> sort a copy here

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// core idea: sort, fix the smallest value, close two pointers on the rest
// skip equal values at both levels so no triplet repeats
// T: O(n^2), S: O(1) + output
vector<vector<int>> threeSum(vector<int> nums) {
    sort(nums.begin(), nums.end());
    // result = triplets, each ascending
    vector<vector<int>> result;
    int n = nums.size();
    for (int i = 0; i < n - 2; i++) {
        // same first value -> same triplets
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
                // skip equal left values so the same pair is not added again
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

// follow-up:
// 3Sum closest (LC 16) -> same loops; keep the sum w/ min |sum - target|
// cnt triplets w/ sum < target (LC 259) -> if sum < target: add right - left, left++
// 4Sum / kSum (LC 18) -> fix k - 2 values w/ dup skips; O(n^(k-1)), long long sum
