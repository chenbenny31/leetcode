// LC 560: count the contiguous non-empty subarrays of nums (values may be negative)
// whose sum equals k.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// straightforward: fix the start, extend the end with a running sum.
// T: O(n^2), S: O(1).
int subarraySum(const vector<int>& nums, int k) {
    int n = nums.size();
    int count = 0;
    for (int i = 0; i < n; i++) {
        int sum = 0;   // sum of nums[i..j]
        for (int j = i; j < n; j++) {
            sum += nums[j];
            // no early break: negatives mean the sum can come back to k
            if (sum == k) { count++; }
        }
    }
    return count;
}

// improved: prefix sums; a subarray ending here sums to k iff an earlier prefix equals
// sum - k, so count those prefixes in a hash map. T: O(n), S: O(n).
int subarraySumMap(const vector<int>& nums, int k) {
    // seen[p] = earlier prefixes with sum p; the empty prefix counts once
    unordered_map<int, int> seen{{0, 1}};
    int sum = 0, count = 0;
    for (int x : nums) {
        sum += x;
        auto it = seen.find(sum - k);
        if (it != seen.end()) { count += it->second; }
        seen[sum]++;   // after the lookup, so k == 0 never counts an empty subarray
    }
    return count;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    bool ok = subarraySum(nums, k) == want && subarraySumMap(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {1, 2, 1}, 3, 2);
    test("neg", {1, -1, 0}, 0, 3);
    test("none", {1, 2}, 4, 0);
}
