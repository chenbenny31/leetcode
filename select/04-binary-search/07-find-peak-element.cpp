// LC 162 Find Peak Element (Medium) — binary search on the slope
// return idx of any peak (> both neighbors); nums[-1] = nums[n] = -inf; O(log n)
//
// clarify:
// adjacent values -> differ, so the slope is never flat
// several peaks -> any one is accepted
// ends -> can be peaks
// single element -> idx 0

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: compare mid w/ mid + 1, move to the higher side; a peak stays in [lo, hi]
// T: O(log n), S: O(1)
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

// follow-up:
// 2D peak (LC 1901) -> binary search on cols: max of col mid, move to larger neighbor
// peak of a mountain array (LC 852) -> same loop, the single peak is the answer
