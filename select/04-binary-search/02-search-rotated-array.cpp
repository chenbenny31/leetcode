// LC 33 Search in Rotated Sorted Array (Medium) — binary search, one half is sorted
// ascending distinct nums rotated at unknown pivot; return idx of target or -1; O(log n)
//
// clarify:
// distinct values? -> yes; dups = follow-up (LC 81)
// rotated 0 times -> ok, plain sorted array
// empty array -> -1
// return -> idx

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: one half of [lo, hi] around mid is always sorted
// go into it only if target is inside its value range, else the other half
// T: O(log n), S: O(1)
int search(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) {
            return mid;
        }
        // <=: when lo == mid the left half is the single sorted value nums[lo]
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
    cout << label << (search(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("right-half", {5, 6, 7, 1, 2, 3}, 2, 4);
    test("left-half", {5, 6, 7, 1, 2, 3}, 6, 1);
    test("missing", {5, 6, 7, 1, 2, 3}, 4, -1);
    test("two-elements", {3, 1}, 1, 1);
    return 0;
}

// follow-up:
// dups allowed (LC 81) -> if nums[lo] == nums[mid] == nums[hi]: lo++, hi--; O(n) worst
// two-pass version -> find idx of min (LC 153), then binary search the part w/ target
