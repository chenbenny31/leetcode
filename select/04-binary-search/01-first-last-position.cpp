// LC 34 Find First and Last Position of Element in Sorted Array (Medium). Pattern:
// lower-bound template.
// nums sorted ascending; return [first, last] index of target, or [-1, -1], in O(log n).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Lower-bound template: first index i with nums[i] >= target (n if none).
// The answer always lies in [lo, hi]; hi starts at n so "none" is a valid answer.
// T: O(log n), S: O(1).
int lowerBound(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = nums.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            // mid and everything left of it are too small
            lo = mid + 1;
        } else {
            // mid is a candidate; the answer is mid or left of it
            hi = mid;
        }
    }
    return lo;
}

// first = lowerBound(target), last = lowerBound(target + 1) - 1. T: O(log n), S: O(1).
vector<int> searchRange(const vector<int>& nums, int target) {
    int n = nums.size();
    int first = lowerBound(nums, target);
    if (first == n || nums[first] != target) {
        return {-1, -1};
    }
    // last target sits just before the first value > target
    int last = lowerBound(nums, target + 1) - 1;
    return {first, last};
}

void test(const string& label, const vector<int>& nums, int target,
          const vector<int>& want) {
    cout << label << (searchRange(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dups", {1, 2, 2, 2, 3}, 2, {1, 3});
    test("all-same", {2, 2}, 2, {0, 1});
    test("missing-gap", {1, 3}, 2, {-1, -1});
    test("empty", {}, 1, {-1, -1});
}

// follow-up: upper bound (first > target) -> same loop with nums[mid] <= target;
//   count of target = upperBound - lowerBound; insert position (LC 35) = lowerBound
// follow-up: "first index where pred is true" for any monotonic pred -> same template,
//   replace nums[mid] < target with !pred(mid)
