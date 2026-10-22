// LC 540 Single Element in a Sorted Array: sorted nums where every value appears twice
// except one that appears once. Return it in O(log n) time and O(1) space.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Binary search on index parity: before the single element each pair starts at an even
// index, after it at an odd index. Compare mid (made even) with its right neighbor.
// T: O(log n), S: O(1).
int singleNonDuplicate(const vector<int>& nums) {
    int lo = 0;
    int hi = nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (mid % 2 == 1) { mid--; }
        if (nums[mid] == nums[mid + 1]) {
            lo = mid + 2;
        } else {
            hi = mid;
        }
    }
    return nums[lo];
}

void test(const string& label, const vector<int>& nums, int want) {
    if (singleNonDuplicate(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("middle", {1, 1, 2, 3, 3}, 2);
    test("first", {1, 2, 2}, 1);
    test("last", {1, 1, 2, 2, 3}, 3);
    test("single", {7}, 7);
}
