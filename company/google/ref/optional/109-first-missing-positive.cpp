// 109 · First Missing Positive — LC 41
// Mother problem: select/optional/01-arrays-hashing/07-first-missing-positive.cpp
//
// Google reports: LC tag #68
//
// Clarify
// - values? → any ints, negatives and zeros included; duplicates allowed
// - may I modify the input? → yes (required for O(1) extra space)
// - constraints? → O(n) time, O(1) extra space
// - all 1..n present? → return n + 1
//
// Core idea
// - The answer is in 1..n + 1, so the array itself is the n-slot set.
// - Cyclic sort: while nums[i] is in 1..n and nums[nums[i] − 1] != nums[i], swap it
//   home; each swap places one value for good, so ≤ n swaps total.
// - First slot i with nums[i] != i + 1 gives i + 1; else n + 1.
// - T O(n), S O(1).
//
// Tests: gap, all present, too big, dups
//
// Follow-ups
// - why not a hash set → O(n) space
// - sign marking → replace non-positives with n + 1, mark v present by negating nums[v −
//   1], first positive slot is the answer
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// Answer is in 1..n+1, so put each value v in 1..n at slot v - 1, then find the first
// slot that misfits. T: O(n), S: O(1).
int firstMissingPositive(vector<int>& nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // each swap puts one value home for good: at most n swaps in total;
        // the home-slot check also stops a duplicate from swapping forever
        while (nums[i] >= 1 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
            swap(nums[i], nums[nums[i] - 1]);
        }
    }
    for (int i = 0; i < n; i++) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }
    return n + 1;
}

void test(const string& label, vector<int> nums, int want) {
    cout << label << (firstMissingPositive(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("gap", {2, -5, 1, 4}, 3);
    test("all-present", {3, 1, 2}, 4);
    test("too-big", {5, 6}, 1);
    test("dups", {1, 1}, 2);
}
