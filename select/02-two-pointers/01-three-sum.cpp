// LC 15 3Sum (Medium). Pattern: sort + fix one + two pointers.
// Return all unique triplets [a, b, c] of values from nums (distinct indices) with
// a + b + c == 0; no duplicate triplets.

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

// follow-up: 3Sum closest (LC 16) -> same loop, track sum with min |sum - target|
// follow-up: 4Sum / kSum (LC 18) -> fix k-2 values with dup skips, two pointers inside,
//   O(n^(k-1)); use long long for the sum
