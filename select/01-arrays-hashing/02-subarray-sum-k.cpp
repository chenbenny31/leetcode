// LC 560 Subarray Sum Equals K (Medium). Pattern: prefix sum + hash map count.
// Count the contiguous non-empty subarrays of nums whose sum equals k; values may be
// negative, so a sliding window does not work.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// A subarray ending here sums to k iff an earlier prefix equals prefix - k.
// T: O(n), S: O(n).
int subarraySum(const vector<int>& nums, int k) {
    // prefixCount[p] = earlier prefixes with sum p; the empty prefix counts once
    unordered_map<int, int> prefixCount{{0, 1}};
    int prefix = 0;
    int count = 0;
    for (int num : nums) {
        prefix += num;
        auto it = prefixCount.find(prefix - k);
        if (it != prefixCount.end()) {
            count += it->second;
        }
        // after the lookup, so k == 0 never counts an empty subarray
        prefixCount[prefix]++;
    }
    return count;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (subarraySum(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap", {1, 2, 1}, 3, 2);
    test("negative-zero-k", {1, -1, 0}, 0, 3);
    test("none", {1, 2}, 5, 0);
}

// follow-up: all positive -> sliding window, shrink while sum > k, count when == k
// follow-up: longest subarray with sum k (LC 325) -> store first index of each prefix
//   only, best = max(i - first[prefix - k]), O(n)
// follow-up: sum divisible by k (LC 974) -> key ((prefix % k) + k) % k, same counting
