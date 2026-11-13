// LC 1 Two Sum (Easy) — hash map of earlier values
// return idx of the 2 nums that add up to target; same element can't be used twice
//
// clarify:
// exactly one answer? -> yes; return {} if none anyway
// dup values -> ok, look up before insert
// negative values -> ok
// order of the 2 idx -> smaller first
// sorted input? -> follow-up (LC 167)

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// core idea: one pass; look up target - nums[i] among earlier values, then record nums[i]
// T: O(n), S: O(n)
vector<int> twoSum(const vector<int>& nums, int target) {
    // seen[value] = idx of an earlier element w/ that value
    unordered_map<int, int> seen;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // look up before insert, so nums[i] never pairs w/ itself
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

// follow-up:
// sorted input, O(1) space (LC 167) -> two pointers from both ends
// values in small range [lo, hi] -> array indexed by value - lo replaces the map
// 3 values sum to 0 (LC 15) -> 02-two-pointers/01-three-sum (sort, fix one)
// contains dup (LC 217) -> hash set of seen values
// dup within distance k (LC 219) -> map value -> last idx, check i - last <= k
