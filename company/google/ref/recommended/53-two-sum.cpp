// 53 · Two Sum — LC 1, 217, 219
// Mother problem: select/01-arrays-hashing/01-two-sum.cpp
//
// Google reports: LC 1 ×2
//
// LC 1: return the indices of the two numbers in nums that add up to target. Exactly one
// answer exists; the same element can't be used twice.
//
// Clarify
// - equal values at two indices are fine; negatives allowed
// - return the two indices, earlier one first
//
// Core idea
// - One pass with a hash map value → index of earlier elements.
// - Look up target - x before inserting x, so x never pairs with itself.
// - T O(n), S O(n).
//
// Tests
// - basic; 4 must not pair with itself; duplicates {5, 2, 5}, 10; negative target
//
// Follow-ups
// - sorted input (LC 167) → two pointers from both ends: sum too small → left++, too
//   big → right--, O(1) space
// - values in a small range [lo, hi] → vector seen(hi − lo + 1, −1) indexed by v − lo
// - contains duplicate (LC 217) → hash set, true when an insert finds the value
// - contains duplicate II (LC 219) → hash map value → last index, true if i − last <= k

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// One pass: look up target - x among earlier values, then record x. T: O(n), S: O(n).
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
}
