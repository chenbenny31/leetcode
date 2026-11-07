// LC 153 Find Minimum in Rotated Sorted Array: minimum of a rotated sorted array of
// distinct values, in O(log n).
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Half-open search: if nums[mid] > nums[hi] the min is right of mid, else at or left.
// T: O(log n), S: O(1).
int findMin(const vector<int>& nums) {
    int n = nums.size();
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] > nums[hi]) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return nums[lo];
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (findMin(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("rotated", {5, 6, 1, 2, 3, 4}, 1);
    test("not-rotated", {1, 2, 3}, 1);
    test("two-elements", {2, 1}, 1);
    test("single", {7}, 7);
}
