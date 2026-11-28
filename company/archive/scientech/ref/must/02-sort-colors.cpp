// 02 · Sort Colors — LC 75 (Medium), Quant Dev 2023 + SWE 2022
// Dutch national flag: nums holds only 0s, 1s, 2s; sort in place
//
// clarify:
// one pass? -> yes; two passes = counting (follow-up 1)
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

// core idea: three regions grow toward the middle; swap nums[i] into its region
// T: O(n), S: O(1)
void sortColors(vector<int>& nums) {
    int lo = 0;                    // [0, lo) holds 0s
    int i = 0;                     // [lo, i) holds 1s, [i, hi] not seen yet
    int hi = (int)nums.size() - 1; // (hi, n - 1] holds 2s
    while (i <= hi) {
        if (nums[i] == 0) {
            // nums[lo] is a 1 already seen (or i itself), so i can move on
            swap(nums[lo], nums[i]);
            lo++;
            i++;
        } else if (nums[i] == 2) {
            // the value swapped in from hi is not seen yet, so i stays
            swap(nums[i], nums[hi]);
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
    test("empty", {}, {});
    test("mixed", {1, 2, 0, 2, 1, 0}, {0, 0, 1, 1, 2, 2});
    test("swapped-in-two", {2, 2, 0}, {0, 2, 2});
}

// follow-up:
// 1. two passes allowed -> count the 0s, 1s, 2s, then overwrite; O(n)
// 2. k colors -> counting sort O(n + k), or split by the mid color and recurse O(n log k)
// 3. why i stays after a swap with hi -> the value from hi has not been checked yet
