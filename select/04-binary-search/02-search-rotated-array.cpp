// LC 33 Search in Rotated Sorted Array (Medium). Pattern: binary search, one sorted half.
// nums is ascending with distinct values, rotated at an unknown pivot; return the index
// of target, or -1, in O(log n).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// One half of [lo, hi] around mid is always sorted; keep it only if target lies inside
// its range. T: O(log n), S: O(1).
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

// LC 81, duplicates allowed: same as search, but if nums[lo] == nums[mid] == nums[hi]
// neither half is known sorted, so shrink both ends by one.
// T: O(log n) typical, O(n) worst case (e.g. all equal), S: O(1).
bool searchWithDups(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) {
            return true;
        }
        if (nums[lo] == nums[mid] && nums[mid] == nums[hi]) {
            // both ends equal nums[mid] != target, so dropping them loses nothing
            lo++;
            hi--;
        } else if (nums[lo] <= nums[mid]) {
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
    return false;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (search(nums, target) == want ? " PASS\n" : " FAIL\n");
}

void testDups(const string& label, const vector<int>& nums, int target, bool want) {
    cout << label << (searchWithDups(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("right-half", {5, 6, 7, 1, 2, 3}, 2, 4);
    test("left-half", {5, 6, 7, 1, 2, 3}, 6, 1);
    test("missing", {5, 6, 7, 1, 2, 3}, 4, -1);
    test("two-elements", {3, 1}, 1, 1);
    testDups("dups-ends-equal", {1, 1, 1, 2, 1}, 2, true);
    testDups("dups-pivot-left", {3, 1, 2, 3, 3}, 1, true);
    testDups("dups-missing", {2, 2, 3, 0, 2}, 1, false);
}

// follow-up: two-pass -> find the min index (LC 153), then plain binary search on the
//   sorted part that can hold target
