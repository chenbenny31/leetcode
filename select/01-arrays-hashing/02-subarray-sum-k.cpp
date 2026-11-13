// LC 560 Subarray Sum Equals K (Medium) — prefix sum + hash map of prefix counts
// Count the contiguous non-empty subarrays of nums whose sum equals k.
// Clarify: negative values and zeros (then no sliding window); k == 0 or k < 0;
//   overlapping subarrays each count; empty nums -> 0; prefix sums fit in int.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// Core idea: sum(i..j) = prefix(j + 1) - prefix(i), so the subarrays ending here that
// sum to k match the earlier prefixes equal to prefix - k. T: O(n), S: O(n).
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
    test("negative-k", {-1, 1, -1, -1}, -1, 5);
    test("none", {1, 2}, 5, 0);
    return 0;
}

// follow-up (ideas only):
// - all values positive -> sliding window in O(1) space: shrink left while the sum is
//   above k; each right end has at most one matching left end
// - longest subarray with sum k (LC 325), equal 0s and 1s (LC 525, 0 counts as -1), or
//   average exactly mean (subtract mean) -> keep the FIRST index of each prefix
// - sum divisible by k (LC 974); multiple of k with length >= 2 (LC 523) -> key the map
//   by the remainder ((prefix % k) + k) % k; LC 523 keeps the first index, empty at -1
// - shortest subarray with sum % mod == k -> need remainder (prefix - k + mod) % mod;
//   keep the LATEST index of each remainder
// - submatrices with sum k (LC 1074) -> fix top and bottom rows, collapse the band into
//   column sums, run this count on that row; O(m^2 * n)
// - remove one element, then split into two equal-sum parts -> the removed x is
//   total - 2 * (smaller side); look it up in a set of the other side's values
// - range updates: flight bookings (LC 1109), max rectangle overlap -> difference array
//   (+ at the start, - just past the end; four corners in 2D), then prefix sums
