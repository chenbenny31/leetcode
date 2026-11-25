// 60 · Two Sum — LC 1, 217, 219
// Mother: select/01-arrays-hashing/01-two-sum.cpp
// return idx of the two nums that add up to target; exactly one answer exists
// reported: LC 1 ×2
//
// clarify:
// same element twice -> not allowed
// equal values at two idx -> ok, they can pair
// negatives -> ok
// order of the two idx -> smaller first

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// core idea: map value -> idx of earlier elements; look up target - x before inserting x
// T: O(n), S: O(n)
vector<int> twoSum(const vector<int>& nums, int target) {
    // seen[value] = index of an earlier element with that value
    unordered_map<int, int> seen;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
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

// follow-up:
// sorted input (LC 167) -> two pointers from both ends, O(1) space
// values in a small range [lo, hi] -> vector seen(hi - lo + 1, -1) indexed by v - lo
// contains dup (LC 217) -> hash set, true when an insert finds the value
// contains dup II (LC 219) -> map value -> last idx, true if i - last <= k
