// ByteDance (FastPrep #23, intern phone) / LC 81 variant: nums is a non-decreasing
// array (duplicates allowed) rotated at an unknown pivot (possibly 0). Return the
// smallest index holding target, or -1. O(n) worst case is allowed.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Index where the original order starts: the "drop" nums[i-1] > nums[i], or 0 if none.
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
            // equal ends hide the side: shrink hi, but first check hi is not the drop
            if (nums[hi - 1] > nums[hi]) {
                return hi;
            }
            hi--;
        }
    }
    return lo;
}

// Leftmost index of target in the sorted range [lo, hi], or -1.
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

// Find the drop, then binary search the two sorted pieces, left piece first.
// T: O(log n), O(n) worst case with duplicates. S: O(1).
int search(const vector<int>& nums, int target) {
    int n = nums.size();
    int start = findStart(nums);
    // equal values can sit in both pieces; the left piece has the smaller indexes
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
    test("missing", {5, 6, 1, 3}, 4, -1);
}

// follow-up: distinct values (LC 33) -> nums[mid] == nums[hi] never happens, strictly
//   O(log n)
// follow-up: why O(n) worst -> {1,1,1,0,1,1}: equal ends hide the drop; any algorithm
//   must read every slot to find the lone 0
// follow-up: minimum / rotation count (LC 154) -> nums[findStart(nums)]; start = how
//   far the sorted array was rotated
