// count the contiguous non-empty subarrays of nums eq k
// vals could be negative

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// fix the start, extend the end with a running sum, T: O(n^2), S: O(1)
int subarraySum(const vector<int>& nums, int k) {
    int n = nums.size();
    int count = 0;
    for (int i = 0; i < n; i++) {
        int sum = 0; // sum of nums[i..j]
        for (int j = i; j < n; j++) {
            sum += nums[j];
            if (sum == k) { count++; }
        }
    }
    return count;
}

// prefix-sum, subarray-sum eq k iff an earlier prefix eq sum - k, count in hash-map
// T: O(n), S: O(n)
int subarraySumMap(const vector<int>& nums, int k) {
    unordered_map<int, int> sumCnt{{0, 1}}; // presum -> its count
    int sum = 0;
    int count = 0;
    for (int num : nums) {
        sum += num;
        auto it = sumCnt.find(sum - k);
        if (it != sumCnt.end()) { count += it->second; }
        sumCnt[sum]++; // update after lookup
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

