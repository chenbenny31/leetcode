// Longest subarray with at most k distinct values (LC 340 on an int array).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::unordered_map;

// Sliding window: grow right; while more than k distinct, shrink left.
// T: O(n), S: O(k).
int longestKDistinct(const vector<int>& nums, int k) {
    int n = nums.size();
    // count[v] = occurrences of v inside the window
    unordered_map<int, int> count;
    int best = 0;
    int left = 0;
    for (int right = 0; right < n; right++) {
        count[nums[right]]++;
        while ((int)count.size() > k) {
            count[nums[left]]--;
            if (count[nums[left]] == 0) { count.erase(nums[left]); }
            left++;
        }
        best = max(best, right - left + 1);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (longestKDistinct(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("shrink", {1, 2, 1, 3}, 2, 3);
    test("k-zero", {1, 2}, 0, 0);
    test("k-above-distinct", {1, 2, 3}, 5, 3);
}
