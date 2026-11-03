// ByteDance: nums is sorted in non-decreasing order (0 <= n <= 1e5). Return the
// smallest index i with nums[i] == target, or -1 if target is absent. O(log n).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Lower bound: the first index with nums[i] >= target always lies in [lo, hi].
// nums[mid] < target means it is right of mid; otherwise mid itself may be the answer,
// so hi = mid keeps it. The loop ends with lo == hi; check that slot holds target.
// T: O(log n), S: O(1).
int leftmostIndex(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = nums.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    if (lo < (int)nums.size() && nums[lo] == target) {
        return lo;
    }
    return -1;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (leftmostIndex(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("all-equal", {5, 5, 5}, 5, 0);
    test("run-in-middle", {1, 3, 3, 7}, 3, 1);
    test("past-end", {1, 2, 3}, 4, -1);
    test("before-start", {2, 4}, 1, -1);
    test("empty", {}, 1, -1);
}
