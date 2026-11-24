// nums hold 0s, 1s, 2s, sort in place

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

void sortColors(vector<int>& nums) {
    int n = nums.size();
    int lo = 0; // [0..lo) is 0
    int hi = n - 1; // (hi..n-1] is 2
    int i = 0; // [lo..i) is 1, [i..hi] is un-class

    while (i <= hi) {
        if (nums[i] == 0) {
            swap(nums[lo], nums[i]);
            lo++;
            i++;
        } else if (nums[i] == 2) {
            swap(nums[hi], nums[i]);
            hi--;
        } else {
            i++;
        }
    }
}

void test(const string& label, vector<int> nums, const vector<int>& want) {
    sortColors(nums);
    cout << label << (nums == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reversed", {2, 1, 0}, {0, 1, 2});
    test("mixed", {1, 2, 0, 2, 1, 0}, {0, 0, 1, 1, 2, 2});
    test("swap-in-two", {2, 2, 0}, {0, 2, 2});
    test("single", {1}, {1});
    return 0;
}

// reverseda, mixed, only 0-2, single elem

// two-pass allow -> counting-sort
// k-colors -> counting-sort or recursive 3-way split O(nlogk)
