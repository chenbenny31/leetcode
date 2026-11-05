// ByteDance / LC 540: in a sorted array every element appears exactly twice except one,
// which appears once. Return it in O(log n) time and O(1) space.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Binary search on even indices. T: O(log n), S: O(1).
int singleNonDuplicate(const vector<int>& nums) {
    int lo = 0;
    int hi = nums.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        // even index: before the single, pairs start at even indices
        mid -= mid % 2;
        if (nums[mid] == nums[mid + 1]) {
            // pair intact: the single is to the right
            lo = mid + 2;
        } else {
            // pair broken: the single is mid or to the left
            hi = mid;
        }
    }
    return nums[lo];
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (singleNonDuplicate(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {1, 1, 2, 3, 3, 4, 4, 8, 8}, 2);
    test("lc-ex2", {3, 3, 7, 7, 10, 11, 11}, 10);
    test("single", {1}, 1);
}

// follow-up: unsorted array (LC 136) -> XOR of all elements, O(n) O(1)
// follow-up: shorter parity check -> compare nums[mid] with nums[mid ^ 1]: equal ->
//   lo = mid + 1, else hi = mid
// follow-up: others appear 3 times (LC 137) -> per bit count % 3, O(32n) O(1)
