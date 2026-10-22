// LC 162 Find Peak Element: a peak is strictly greater than its neighbors; nums[-1] and
// nums[n] count as -infinity and adjacent values differ. Return any peak index in
// O(log n).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Half-open search: if nums[mid] < nums[mid + 1] a peak lies to the right (climb up),
// otherwise at or left of mid. T: O(log n), S: O(1).
int findPeakElement(const vector<int>& nums) {
    int lo = 0;
    int hi = nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < nums[mid + 1]) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& nums) {
    int n = nums.size();
    int p = findPeakElement(nums);
    bool ok = p >= 0 && p < n;
    if (ok && p > 0 && nums[p - 1] > nums[p]) { ok = false; }
    if (ok && p < n - 1 && nums[p + 1] > nums[p]) { ok = false; }
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("one-peak", {1, 3, 2});
    test("two-peaks", {1, 3, 1, 4, 2});
    test("increasing", {1, 2, 3});
    test("single", {5});
}
