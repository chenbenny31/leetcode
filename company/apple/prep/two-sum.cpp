// return indices of two val in arr that add up to target
// only one solution, same elem cannot be used twice

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

// one pass with val->idx map, T: O(n), S: O(n)
vector<int> twoSum(const vector<int>& nums, int target) {
    unordered_map<int, int> seen; // seen[val] = idx
    const int n = nums.size();
    for (int i = 0; i < n; i++) {
        auto it = seen.find(target - nums[i]);
        if (it != seen.end()) { return {it->second, i}; }
        seen[nums[i]] = i;
    }
    return {};
}

// follow-up, T: O(n + R), S: O(R), R: data range
vector<int> twoSumRange(const vector<int>& nums, int target) {
    const int n = nums.size();
    int lo = *min_element(nums.begin(), nums.end());
    int hi = *max_element(nums.begin(), nums.end());

    vector<int> seen(hi - lo + 1, -1);
    for (int i = 0; i < n; i++) {
        int need = target - nums[i];
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
    test("lc ex1", {2, 7, 11, 15}, 9, {0, 1});
    test("not self", {3, 2, 4}, 6, {1, 2});
    test("duplicates", {3, 3}, 6, {0, 1});
}
