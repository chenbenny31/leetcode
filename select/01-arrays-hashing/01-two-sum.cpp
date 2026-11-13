// LC 1 Two Sum (Easy) — hash map of earlier values
// Return the indices of the two numbers in nums that add up to target; the same element
// cannot be used twice.
// Clarify: exactly one answer exists; duplicate values; negative values; order of the
//   two indices; sorted input or not.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// Core idea: one pass; look up target - nums[i] among the earlier values, then record
// nums[i]. T: O(n), S: O(n).
vector<int> twoSum(const vector<int>& nums, int target) {
    // seen[value] = index of an earlier element with that value
    unordered_map<int, int> seen;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // look up before insert, so nums[i] never pairs with itself
        auto it = seen.find(target - nums[i]);
        if (it != seen.end()) {
            return {it->second, i};
        }
        seen[nums[i]] = i;
    }
    return {};
}

void test(const string& label, const vector<int>& nums, int target,
          const vector<int>& want) {
    cout << label << (twoSum(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {2, 7, 5}, 12, {1, 2});
    test("not-self", {4, 1, 7}, 8, {1, 2});
    test("dup", {5, 2, 5}, 10, {0, 2});
    test("negative", {-3, 4, 1}, -2, {0, 2});
    return 0;
}

// follow-up (ideas only):
// - sorted input, O(1) space (LC 167) -> two pointers from both ends: sum too small
//   moves left up, too big moves right down
// - values in a small range [lo, hi] -> an array indexed by value - lo replaces the hash
//   map; check that target - value is in range first
// - three values that sum to 0 (LC 15) -> 02-two-pointers/01-three-sum (sort, fix one)
// - contains duplicate (LC 217) -> hash set of seen values; duplicate within distance k
//   (LC 219) -> map value -> last index, check i - last <= k
