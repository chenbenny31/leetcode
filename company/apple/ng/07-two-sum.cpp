// LC 1: return indices of the two numbers in nums that add up to target.
// Exactly one solution exists; the same element can't be used twice.
// Follow-up: values lie in a small range (e.g. |v| <= 1e4) -> vector instead of hash map.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::min_element;
using std::max_element;

// One pass with a value -> index map. T: O(n), S: O(n).
vector<int> twoSum(const vector<int>& nums, int target) {
    unordered_map<int, int> seen;   // seen[value] = index of an earlier element
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // look up before insert, so nums[i] never pairs with itself
        auto it = seen.find(target - nums[i]);
        if (it != seen.end()) { return {it->second, i}; }
        seen[nums[i]] = i;
    }
    return {};
}

// follow-up: values span a small range [lo, hi], so a vector indexed by v - lo replaces
// the hash map (no hashing, O(1) worst case). T: O(n + range), S: O(range).
vector<int> twoSumRange(const vector<int>& nums, int target) {
    int n = nums.size();
    int lo = *min_element(nums.begin(), nums.end());
    int hi = *max_element(nums.begin(), nums.end());
    // seen[v - lo] = index of an earlier v, -1 = not seen
    vector<int> seen(hi - lo + 1, -1);
    for (int i = 0; i < n; i++) {
        int need = target - nums[i];
        // a complement outside [lo, hi] can't be in nums, and would index out of bounds
        if (need >= lo && need <= hi && seen[need - lo] != -1) {
            return {seen[need - lo], i};
        }
        seen[nums[i] - lo] = i;
    }
    return {};
}

void test(const string& label, const vector<int>& nums, int target,
          const vector<int>& want) {
    cout << label << (twoSum(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {1, 2, 3}, 5, {1, 2});
    test("not-self", {3, 2, 4}, 6, {1, 2});
    test("dup", {3, 3}, 6, {0, 1});
}
