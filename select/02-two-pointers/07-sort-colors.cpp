// LC 75 Sort Colors (Medium) — Dutch national flag, 3-way partition
// nums holds only 0s, 1s, 2s; sort in place
//
// clarify:
// one pass? -> yes; two-pass counting = follow-up
// library sort -> not allowed
// extra space -> O(1)
// values -> only 0, 1, 2

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// core idea: [0, zeroEnd) = 0s, [zeroEnd, cur) = 1s, [cur, twoStart] unseen, rest = 2s
// swap nums[cur] into its region
// T: O(n), S: O(1)
void sortColors(vector<int>& nums) {
    int zeroEnd = 0;
    int cur = 0;
    int twoStart = (int)nums.size() - 1;
    while (cur <= twoStart) {
        if (nums[cur] == 0) {
            // nums[zeroEnd] is a known 1 (or cur itself) -> cur can advance
            swap(nums[zeroEnd], nums[cur]);
            zeroEnd++;
            cur++;
        } else if (nums[cur] == 2) {
            // value from twoStart is unseen -> cur stays
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
    return 0;
}

// follow-up:
// two passes allowed -> count 0s, 1s, 2s, then overwrite; O(n)
// k colors (LintCode 143) -> counting sort O(n + k), or recursive 3-way split O(n log k)
