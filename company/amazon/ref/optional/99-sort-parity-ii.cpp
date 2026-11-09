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

// even walks even slots, odd walks odd slots; a misplaced odd value at an even slot
// always has a misplaced even value at some odd slot to swap with. T: O(n), S: O(1).
void sortArrayByParityII(vector<int>& nums) {
    int n = nums.size();
    int odd = 1;
    for (int even = 0; even < n; even += 2) {
        if (nums[even] % 2 == 0) { continue; }
        while (nums[odd] % 2 == 1) { odd += 2; }
        swap(nums[even], nums[odd]);
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
