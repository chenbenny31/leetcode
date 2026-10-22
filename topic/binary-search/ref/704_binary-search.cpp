// LC 704 Binary Search: given sorted nums (distinct values) and target, return its index
// or -1. O(log n).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Closed-interval binary search for an exact match. T: O(log n), S: O(1).
int search(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = nums.size() - 1;
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
    if (search(nums, target) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("middle", {2, 4, 6, 8}, 6, 2);
    test("missing", {2, 4, 6, 8}, 5, -1);
    test("single", {3}, 3, 0);
    test("empty", {}, 3, -1);
}
