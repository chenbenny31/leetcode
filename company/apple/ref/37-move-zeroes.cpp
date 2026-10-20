// move all 0s in nums to the end in place, keep the order of the non-zeros

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// write pointer, swap each non-zero into the next free slot, T: O(n), S: O(1)
void moveZeroes(vector<int>& nums) {
    int write = 0; // nums[0..write) = non-zeros seen so far, in order
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        if (nums[i] != 0) {
            swap(nums[write], nums[i]);
            write++;
        }
    }
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    moveZeroes(nums);
    if (nums == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("basic", {0, 1, 0, 2}, {1, 2, 0, 0});
    test("all-zero", {0, 0}, {0, 0});
    test("none", {1, 2}, {1, 2});
}
