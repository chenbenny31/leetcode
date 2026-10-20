// return the index of any peak (strictly greater than both neighbors)
// nums[-1] = nums[n] = -inf, adjacent vals differ, must run in O(log n)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// binary search on the slope, always move toward the higher neighbor
// T: O(log n), S: O(1)
int findPeakElement(const vector<int>& nums) {
    int lo = 0;
    int hi = nums.size() - 1; // some peak lies in [lo, hi]
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < nums[mid + 1]) {
            lo = mid + 1; // rising, a peak is on the right
        } else {
            hi = mid; // falling, mid or its left is a peak
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& nums, int want) {
    if (findPeakElement(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("mid", {1, 3, 2}, 1);
    test("left", {3, 2, 1}, 0);
    test("right", {1, 2, 3}, 2);
    test("one", {5}, 0);
}
