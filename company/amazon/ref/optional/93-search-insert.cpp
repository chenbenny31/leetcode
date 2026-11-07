// LC 35: nums is sorted with distinct values. Return target's index, or the index where
// it would be inserted to keep the order.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// binary search for the first index with nums[i] >= target. T: O(log n), S: O(1).
int searchInsert(const vector<int>& nums, int target) {
    int lo = 0, hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (searchInsert(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("found", {1, 3, 5}, 3, 1);
    test("between", {1, 3, 5}, 4, 2);
    test("front", {1, 3, 5}, 0, 0);
    test("back", {1, 3, 5}, 9, 3);
    test("empty", {}, 2, 0);
}
