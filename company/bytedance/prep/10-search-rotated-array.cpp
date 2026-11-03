// vals uniq? yes, dup are follow-up
// return idx or bool? idx or -1 if absent
// can rotated by 0? yes
// empty? no

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// binary-search, mid comp with hi, T: O(logn), S: O(1)
int search(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) { return mid; }
        if (nums[lo] <= nums[mid]) { // left sorted half, == iff lo==mid
            if (nums[lo] <= target && target < nums[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else { // right sorted half
            if (nums[mid] < target && target <= nums[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return -1;
}

// follow-up, with dup, T: O(logn) or O(n) worst all equal, S: O(1)
bool searchDup(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = (int)nums.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) { return true; }
        if (nums[lo] == nums[mid] && nums[mid] == nums[hi]) {
            lo++;
            hi--;
        } else if (nums[lo] <= nums[mid]) {
            if (nums[lo] <= target && target < nums[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
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

void testDup(const string& label, const vector<int>& nums, int target, bool want) {
    cout << label << (searchDup(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {4, 5, 6, 7, 0, 1, 2}, 0, 4);
    test("missing", {4, 5, 6, 7, 0, 1, 2}, 3, -1);
    test("single", {1}, 1, 0);
    test("not rotated", {1, 3, 5}, 5, 2);
    testDup("dup found", {2, 5, 6, 0, 0, 1, 2}, 0, true);
    testDup("dup missing", {2, 5, 6, 0, 0, 1, 2}, 3, false);
    testDup("ends equal", {1, 0, 1, 1, 1}, 0, true);
}
