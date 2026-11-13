// LC 53 Maximum Subarray (Medium) — Kadane: best sum ending here
// return the largest sum of a non-empty contiguous subarray of nums
//
// clarify:
// empty input -> ask; here 0
// all negative -> answer = largest element, not 0
// sum overflow -> assume fits in int, else long long
// sum or the subarray itself? -> sum; idx -> follow-up

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// core idea: ending = max(x, ending + x): extend the best ending before x or start fresh
// T: O(n), S: O(1)
int maxSubArray(const vector<int>& nums) {
    if (nums.empty()) { return 0; }
    // best sum of a subarray ending exactly at the current element
    int ending = nums[0];
    int best = nums[0];
    for (int i = 1; i < (int)nums.size(); i++) {
        ending = max(nums[i], ending + nums[i]);
        best = max(best, ending);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxSubArray(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle-run", {-2, 3, -1, 4, -5}, 6);
    test("all-negative", {-3, -1, -2}, -1);
    test("whole-array", {1, 2, 3}, 6);
    test("single", {5}, 5);
    return 0;
}

// follow-up:
// return the subarray idx -> remember where the current run started; copy on new best
// circular array (LC 918) -> max(kadane, total - min subarray); all negative -> kadane
// max product subarray (LC 152) -> track max and min product ending here (negative swaps)
// max alternating subsequence sum (LC 1911) -> two states: last pick added / subtracted
// at most one deletion (LC 1186) -> two states: no deletion / one deletion ending here
// 2D max sum submatrix -> fix top / bottom rows, Kadane on column sums: O(rows^2 * cols)
