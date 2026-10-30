// LC 922: nums has as many even as odd values. Reorder it so nums[i] is even when i is
// even and odd when i is odd (any valid answer).

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// i walks even slots, j odd slots; a misplaced odd at i always has a misplaced even at
// some j to swap with. T: O(n), S: O(1).
void sortArrayByParityII(vector<int>& nums) {
    int n = nums.size(), j = 1;
    for (int i = 0; i < n; i += 2) {
        if (nums[i] % 2 == 0) { continue; }
        while (nums[j] % 2 == 1) { j += 2; }
        swap(nums[i], nums[j]);
    }
}

bool valid(const vector<int>& nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        if (nums[i] % 2 != i % 2) { return false; }
    }
    return true;
}

void test(const string& label, vector<int> nums) {
    sortArrayByParityII(nums);
    cout << label << (valid(nums) ? " PASS\n" : " FAIL\n");
}

int main() {
    test("swapped", {1, 2, 3, 4});
    test("all-wrong", {3, 1, 4, 2});
    test("done", {2, 1});
}
