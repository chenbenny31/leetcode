// 114 · First Missing Positive — LC 41
// Mother: select/optional/01-arrays-hashing/07-first-missing-positive.cpp
// return smallest positive int missing from nums; O(n) time, O(1) extra space
// reported: LC tag #68
//
// clarify:
// negatives, zeros, dups -> allowed
// modify input? -> yes, needed for O(1) space
// all of 1..n present -> n + 1

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// core idea: answer is in 1..n + 1; cyclic-sort each v in 1..n to slot v - 1
// first slot i w/ nums[i] != i + 1 gives i + 1
// T: O(n), S: O(1) — each swap puts one value home for good, <= n swaps
int firstMissingPositive(vector<int>& nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // home-slot check also stops a dup from swapping forever
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

// follow-up:
// why not a hash set -> O(n) space
// sign marking -> non-positives = n + 1, negate slot v - 1; first positive slot = answer
