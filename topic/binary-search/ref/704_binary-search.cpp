// LC 704 Binary Search: index of target in sorted distinct nums, or -1, in O(log n).
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Closed-interval binary search for an exact match. T: O(log n), S: O(1).
int search(const vector<int>& nums, int target) {
    int n = nums.size();
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) { return mid; }
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    return -1;
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (search(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {2, 4, 6, 8}, 6, 2);
    test("missing", {2, 4, 6, 8}, 5, -1);
    test("single", {3}, 3, 0);
    test("empty", {}, 3, -1);
}
