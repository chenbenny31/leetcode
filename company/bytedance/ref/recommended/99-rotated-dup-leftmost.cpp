// ByteDance (FastPrep #23, intern phone) / LC 81 variant: nums was sorted in
// non-decreasing order (duplicates allowed) and then rotated at an unknown pivot
// (possibly by 0). Return the smallest index whose value equals target, or -1.
// Use the rotated-sorted structure to discard ranges; equal boundary values can hide
// which side is sorted, so O(n) is allowed in the worst case. 1 <= n <= 1e5.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Index where the original sorted order starts: the single "drop" nums[i-1] > nums[i],
// or 0 if there is none. Like finding the minimum of a rotated array, but when
// nums[mid] == nums[hi] we cannot tell the side, so we shrink hi by one - after first
// checking that hi itself is not the drop. T: O(log n), O(n) worst case with duplicates.
int findStart(const vector<int>& nums) {
    int lo = 0;
    int hi = nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] > nums[hi]) {
            // the drop lies after mid
            lo = mid + 1;
        } else if (nums[mid] < nums[hi]) {
            // mid..hi is sorted, so the drop is at mid or before it
            hi = mid;
        } else {
            if (nums[hi - 1] > nums[hi]) {
                return hi;
            }
            hi--;
        }
    }
    return lo;
}

// Leftmost index of target in the sorted range [lo, hi], or -1. T: O(log n).
int leftmost(const vector<int>& nums, int lo, int hi, int target) {
    int found = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            if (nums[mid] == target) {
                found = mid;
            }
            hi = mid - 1;
        }
    }
    return found;
}

// Split at the start of the original order into two sorted pieces: [0, start) holds the
// larger values, [start, n) the smaller ones. A value can sit in both pieces (equal
// values at the wrap), so search the left piece first - its indexes are smaller.
// T: O(log n), O(n) worst case. S: O(1).
int search(const vector<int>& nums, int target) {
    int n = nums.size();
    int start = findStart(nums);
    int inLeft = leftmost(nums, 0, start - 1, target);
    if (inLeft != -1) {
        return inLeft;
    }
    return leftmost(nums, start, n - 1, target);
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (search(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-rotation", {1, 2, 2, 3}, 2, 1);
    test("both-pieces", {3, 3, 1, 2, 3}, 3, 0);
    test("right-piece", {4, 5, 1, 1, 2}, 1, 2);
    test("hidden-drop", {2, 2, 2, 0, 2}, 0, 3);
    test("all-same", {7, 7, 7}, 7, 0);
    test("missing", {5, 6, 1, 3}, 4, -1);
}
