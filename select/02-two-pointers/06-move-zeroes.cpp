// LC 283 Move Zeroes (Easy) — read / write pointers, swap forward
// move all 0s to the end in place, keep order of non-zero values
//
// clarify:
// in place? -> yes, no copy of the array
// order of non-zeros -> kept
// negative values -> ok, only 0 is special
// min num of writes? -> not required here

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// core idea: swap each non-zero into the next write slot; nums[write..] ends all 0
// T: O(n), S: O(1)
void moveZeroes(vector<int>& nums) {
    // nums[0..write) = non-zeros seen so far, in order
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

// follow-up:
// remove all copies of val (LC 27) -> same write ptr, copy instead of swap, return write
// remove dups from sorted array (LC 26) -> copy nums[read] if != nums[write - 1]
