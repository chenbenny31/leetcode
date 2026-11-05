// ByteDance / LC 560: count the contiguous non-empty subarrays of nums (values may be
// negative) whose sum equals k.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// Prefix sums: a subarray ending here sums to k iff an earlier prefix == sum - k.
// T: O(n), S: O(n).
int subarraySum(const vector<int>& nums, int k) {
    // prefixCount[p] = earlier prefixes with sum p; the empty prefix counts once
    unordered_map<int, int> prefixCount{{0, 1}};
    int sum = 0;
    int count = 0;
    for (int x : nums) {
        sum += x;
        auto it = prefixCount.find(sum - k);
        if (it != prefixCount.end()) {
            count += it->second;
        }
        // after the lookup, so k == 0 never counts an empty subarray
        prefixCount[sum]++;
    }
    return count;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (subarraySum(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {1, 2, 1}, 3, 2);
    test("negative", {1, -1, 0}, 0, 3);
    test("none", {1, 2}, 5, 0);
}

// follow-up: all positive -> sliding window, shrink while sum > k, count when == k
// follow-up: longest subarray with sum k (LC 325) -> first[p] = earliest index only,
//   best = max(i - first[sum - k]), O(n)
// follow-up: sum divisible by k (LC 974) -> key ((sum % k) + k) % k, same counting, O(n)
