// LC 154 Find Minimum in Rotated Sorted Array II: like LC 153 but nums may contain
// duplicates. Return the minimum.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Half-open search; when nums[mid] == nums[hi] we cannot tell the side, but dropping hi
// is safe because nums[mid] still holds the same value. T: O(n) worst case, O(log n)
// typical, S: O(1).
int findMin(const vector<int>& nums) {
    int lo = 0;
    int hi = nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] > nums[hi]) {
            lo = mid + 1;
        } else if (nums[mid] < nums[hi]) {
            hi = mid;
        } else {
            hi--;
        }
    }
    return nums[lo];
}

void test(const string& label, const vector<int>& nums, int want) {
    if (findMin(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("dups-at-ends", {2, 2, 0, 1, 2}, 0);
    test("min-hidden-by-dups", {3, 3, 1, 3}, 1);
    test("all-equal", {1, 1}, 1);
    test("not-rotated", {1, 3, 5}, 1);
}
