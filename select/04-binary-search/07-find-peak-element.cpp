// LC 162 Find Peak Element (Medium) — binary search on the slope
// Return the index of any peak (strictly greater than both neighbors), where
// nums[-1] = nums[n] = -inf and adjacent values differ, in O(log n).
// Clarify: adjacent values differ; any peak is accepted; the two ends can be peaks;
//   single element.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: compare mid with mid + 1 and move toward the higher side; a peak always
// stays inside [lo, hi]. T: O(log n), S: O(1).
int findPeakElement(const vector<int>& nums) {
    int lo = 0;
    int hi = (int)nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        // mid < hi, so mid + 1 is in range
        if (nums[mid] < nums[mid + 1]) {
            // rising: climbing right must hit a peak (nums[n] = -inf)
            lo = mid + 1;
        } else {
            // falling: mid or something left of it is a peak
            hi = mid;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (findPeakElement(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {1, 3, 2}, 1);
    test("left-edge", {3, 2, 1}, 0);
    test("right-edge", {1, 2}, 1);
    test("single", {5}, 0);
    return 0;
}

// follow-up (ideas only):
// - 2D peak (LC 1901) -> binary search on columns: take the max of column mid and move
//   toward its larger horizontal neighbor, O(m log n)
