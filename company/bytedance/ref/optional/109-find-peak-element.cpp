// ByteDance / LC 162: return the index of any peak (strictly greater than its
// neighbors), where nums[-1] = nums[n] = -inf and adjacent values differ.
// Must run in O(log n).
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Binary search on the slope: always move toward the higher neighbor.
// T: O(log n), S: O(1).
int findPeakElement(const vector<int>& nums) {
    int n = nums.size();
    // invariant: some peak lies in [lo, hi]
    int lo = 0;
    int hi = n - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < nums[mid + 1]) {
            // rising: climbing right must hit a peak
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
    test("lc-ex1", {1, 2, 3, 1}, 2);
    test("left-edge", {3, 2, 1}, 0);
    test("right-edge", {1, 2}, 1);
}

// follow-up: why binary search is safe -> nums[mid] < nums[mid+1]: climb right, values
//   must turn down before nums[n] = -inf, so a peak lies right of mid
// follow-up: 2D peak (LC 1901) -> binary search rows: col = argmax of row mid;
//   mat[mid][col] < mat[mid+1][col] ? lo = mid + 1 : hi = mid, O(C log R)
// follow-up: equal neighbors allowed -> no O(log n) guarantee (flat runs), worst O(n)
