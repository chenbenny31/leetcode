// LC 152 Maximum Product Subarray (Medium). Pattern: Kadane with max and min.
// Return the largest product of a non-empty contiguous subarray of nums (may contain
// negatives and zeros).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;
using std::swap;

// Track the max and min product ending here; a negative swaps them. T: O(n), S: O(1).
int maxProduct(const vector<int>& nums) {
    // largest / smallest product of a subarray ending at the current element
    int maxHere = nums[0];
    int minHere = nums[0];
    int best = nums[0];
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        int num = nums[i];
        // times a negative, the smallest product becomes the largest
        if (num < 0) {
            swap(maxHere, minHere);
        }
        // num alone restarts the subarray (e.g. after a zero)
        maxHere = max(num, maxHere * num);
        minHere = min(num, minHere * num);
        best = max(best, maxHere);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxProduct(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-negatives", {-2, 3, -4}, 24);
    test("zero-split", {2, 0, 3, 4}, 12);
    test("zero-beats-neg", {-1, 0, -2}, 0);
    test("single-negative", {-3}, -3);
}

// follow-up: max sum subarray (LC 53) -> same scan with only maxHere:
//   maxHere = max(num, maxHere + num)
// follow-up: return the subarray -> remember where maxHere last restarted (== num) and
//   record [start, i] whenever best improves; same for minHere, swapped together
