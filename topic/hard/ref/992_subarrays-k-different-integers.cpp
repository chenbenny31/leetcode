// LC 992 Subarrays with K Different Integers: count the contiguous subarrays of nums that
// contain exactly k distinct integers.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// Count of subarrays with at most k distinct values: for each right end, the valid left
// ends form a contiguous range, so add its length. T: O(n), S: O(n).
int atMost(const vector<int>& nums, int k) {
    unordered_map<int, int> cnt;   // value -> occurrences inside the window
    int left = 0, total = 0;
    for (int right = 0; right < (int)nums.size(); right++) {
        cnt[nums[right]]++;
        while ((int)cnt.size() > k) {
            cnt[nums[left]]--;
            if (cnt[nums[left]] == 0) { cnt.erase(nums[left]); }
            left++;
        }
        total += right - left + 1;
    }
    return total;
}

// exactly k = at most k minus at most k-1. T: O(n), S: O(n).
int subarraysWithKDistinct(vector<int>& nums, int k) {
    return atMost(nums, k) - atMost(nums, k - 1);
}

void test(const string& label, vector<int> nums, int k, int want) {
    cout << label << (subarraysWithKDistinct(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {1, 2, 1, 3}, 2, 4);
    test("single-value", {7, 7, 7}, 1, 6);
    test("k-too-large", {1, 2, 1}, 3, 0);
    test("all-distinct", {1, 2, 3}, 3, 1);
}
