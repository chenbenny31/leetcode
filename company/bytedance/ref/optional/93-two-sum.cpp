// ByteDance / LC 1: return indices of the two numbers in nums that add up to target.
// Exactly one solution exists; the same element can't be used twice.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// One pass with a value -> index map. T: O(n), S: O(n).
vector<int> twoSum(const vector<int>& nums, int target) {
    // seen[value] = index of an earlier element
    unordered_map<int, int> seen;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // look up before insert, so nums[i] never pairs with itself
        int need = target - nums[i];
        if (seen.count(need)) { return {seen[need], i}; }
        seen[nums[i]] = i;
    }
    return {};
}

void test(const string& label, const vector<int>& nums, int target,
          const vector<int>& want) {
    cout << label << (twoSum(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {2, 7, 11, 15}, 9, {0, 1});
    test("not-self", {3, 2, 4}, 6, {1, 2});
    test("duplicates", {3, 3}, 6, {0, 1});
}

// follow-up: sorted input (LC 167) -> two pointers lo, hi; sum < target ? lo++ : hi--,
//   O(n) time, O(1) space
// follow-up: all unique pairs -> sort + two pointers, skip equal values after a match,
//   O(n log n)
// follow-up: 3Sum (LC 15) -> sort, fix i, two pointers on i+1..n-1, skip dups, O(n^2)
// follow-up: count pairs -> cnt[value]; ans += cnt[target - x] before cnt[x]++, O(n)
