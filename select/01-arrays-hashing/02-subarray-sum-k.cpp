// LC 560 Subarray Sum Equals K (Medium) — prefix sum + hash map of prefix counts
// count contiguous non-empty subarrays of nums w/ sum = k
//
// clarify:
// negatives / zeros -> yes, so no sliding window
// k = 0 or k < 0 -> ok, same code
// overlapping subarrays -> each counts
// empty nums -> 0
// overflow -> assume prefix sums fit in int

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// core idea: sum(i..j) = prefix(j + 1) - prefix(i); count earlier prefixes = prefix - k
// T: O(n), S: O(n)
int subarraySum(const vector<int>& nums, int k) {
    // prefixCount[p] = num of earlier prefixes w/ sum p; empty prefix counts once
    unordered_map<int, int> prefixCount{{0, 1}};
    int prefix = 0;
    int count = 0;
    for (int num : nums) {
        prefix += num;
        auto it = prefixCount.find(prefix - k);
        if (it != prefixCount.end()) {
            count += it->second;
        }
        // insert after the lookup, so k = 0 never counts an empty subarray
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

// follow-up:
// all values positive -> sliding window, O(1) space
// longest subarray w/ sum k (LC 325) -> keep FIRST idx of each prefix
// equal 0s and 1s (LC 525) -> 0 counts as -1, longest w/ sum 0
// sum divisible by k (LC 974) -> key the map by ((prefix % k) + k) % k
// multiple of k, len >= 2 (LC 523) -> remainder -> first idx, empty prefix at -1
// shortest subarray w/ sum % mod = k -> need (prefix - k + mod) % mod, keep LATEST idx
// submatrices w/ sum k (LC 1074) -> fix top + bottom rows, run this on col sums
// range updates (LC 1109) -> difference array (+ at start, - past end), then prefix sums
