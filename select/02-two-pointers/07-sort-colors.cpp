// LC 75 Sort Colors (Medium). Pattern: Dutch national flag, 3-way partition.
// nums holds only 0s, 1s and 2s; sort it in place in one pass without a library sort.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// [0, zeroEnd) = 0s, [zeroEnd, cur) = 1s, [cur, twoStart] unseen,
// (twoStart, n-1] = 2s. T: O(n), S: O(1).
void sortColors(vector<int>& nums) {
    int zeroEnd = 0;
    int cur = 0;
    int twoStart = (int)nums.size() - 1;
    while (cur <= twoStart) {
        if (nums[cur] == 0) {
            // nums[zeroEnd] is a known 1 (or cur itself), so cur can advance
            swap(nums[zeroEnd], nums[cur]);
            zeroEnd++;
            cur++;
        } else if (nums[cur] == 2) {
            // the value from twoStart is unseen: re-check it, so cur stays
            swap(nums[cur], nums[twoStart]);
            twoStart--;
        } else {
            cur++;
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
}

// follow-up: two passes allowed -> count 0s, 1s, 2s, then overwrite, O(n)
// follow-up: k colors (LintCode 143) -> counting sort O(n + k), or recursive 3-way
//   partition on the middle color, O(n log k)
