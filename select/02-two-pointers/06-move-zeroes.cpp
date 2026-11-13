// LC 283 Move Zeroes (Easy) — read / write pointers, swap forward
// Move all 0s in nums to the end in place, keeping the order of the non-zero values.
// Clarify: in place, no copy of the array; order of non-zeros kept; negative values;
//   minimize the number of writes or not.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// Core idea: swap each non-zero into the next write slot; everything left behind the
// write pointer is 0. T: O(n), S: O(1).
void moveZeroes(vector<int>& nums) {
    // nums[0..write) = the non-zeros seen so far, in order
    int write = 0;
    int n = nums.size();
    for (int read = 0; read < n; read++) {
        if (nums[read] != 0) {
            swap(nums[write], nums[read]);
            write++;
        }
    }
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    moveZeroes(nums);
    cout << label << (nums == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {0, 1, 0, 2, 3}, {1, 2, 3, 0, 0});
    test("all-zero", {0, 0}, {0, 0});
    test("no-zero", {4, -1}, {4, -1});
    return 0;
}

// follow-up (ideas only):
// - remove all copies of val (LC 27) -> same write pointer, copy instead of swap, return
//   write as the new length
// - remove duplicates from a sorted array (LC 26) -> copy nums[read] when it differs
//   from nums[write - 1]
