// LC 1 Two Sum (Easy). Pattern: hash map of earlier values.
// Return the indices of the two numbers in nums that add up to target. Exactly one
// answer exists; the same element can't be used twice.

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

// follow-up: input sorted (LC 167) -> two pointers from both ends, move the side that
//   makes the sum closer to target, O(1) space
// follow-up: values in a small range [lo, hi] -> vector<int> seen(hi - lo + 1, -1)
//   indexed by v - lo replaces the hash map; check target - v is in range first
