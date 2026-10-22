// LC 33 Search in Rotated Sorted Array: nums was sorted ascending with distinct values,
// then rotated at an unknown pivot. Return the index of target or -1, in O(log n).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Closed-interval binary search; one half around mid is always sorted, so check whether
// target lies inside it. T: O(log n), S: O(1).
int search(const vector<int>& nums, int target) {
    int n = nums.size();
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) { return mid; }
        if (nums[lo] <= nums[mid]) {
            // left half [lo, mid] is sorted
            if (nums[lo] <= target && target < nums[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
            // right half [mid, hi] is sorted
            if (nums[mid] < target && target <= nums[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return -1;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    if (search(nums, target) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("past-pivot", {6, 7, 0, 1, 2, 4, 5}, 1, 3);
    test("before-pivot", {6, 7, 0, 1, 2, 4, 5}, 7, 1);
    test("missing", {6, 7, 0, 1, 2, 4, 5}, 3, -1);
    test("two-elements", {3, 1}, 1, 1);
}
