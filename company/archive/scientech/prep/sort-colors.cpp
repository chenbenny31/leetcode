// nums hold 0s, 1s, 2s, sort in place

// one-pass, two-pass allow counting sort
// no extra space
// values only [0, 1, 2]

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// swap-based one-pass traversal, T: O(n), S: O(1)
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

// reversed, mixed, only 0-2, single elem

// two-pass allow -> counting-sort
// k-colors -> counting-sort or recursive 2-way split O(nlogk)

void sortColorsCounting(vector<int>& nums, int k) {
    vector<int> count(k + 1);
    for (int color : nums) {
        count[color]++;
    }

    int idx = 0;
    for (int color = 1; color <= k; color++) {
        for (int i = 0; i < count[color]; i++) {
            nums[idx] = color;
            idx++;
        }
    }
}

// partition nums[left, right] by mid of color range
void sortRange(vector<int>& nums, int left, int right, int lo, int hi) {
    if (left >= right || lo == hi) { return; }

    int mid = lo + (hi - lo) / 2;
    int cur = left; // [left, cur) colors <= mid
    int split = right; // (split, right] colors > mid

    while (cur <= split) {
        if (nums[cur] <= mid) {
            cur++;
        } else {
            swap(nums[cur], nums[split]);
            split--;
        }
    }

    sortRange(nums, left, split, lo, mid);
    sortRange(nums, cur, right, mid + 1, hi);
}

// rainbow sort, T: O(nlogk), S: O(logk) recursion stack
void sortColorsInPlace(vector<int>& nums, int k) {
    // lo and hi are min/max color values
    sortRange(nums, 0, (int)nums.size() - 1, 1, k);
}
