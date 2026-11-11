// LC 189 Rotate Array (Medium). Pattern: three reversals.
// Rotate nums to the right by k steps in place; n >= 1, k >= 0 and k may exceed n.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::reverse;

// Straightforward: element i moves to (i + k) % n in a copy. T: O(n), S: O(n).
void rotateCopy(vector<int>& nums, int k) {
    int n = nums.size();
    // result[(i + k) % n] = nums[i]
    vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[(i + k) % n] = nums[i];
    }
    nums = result;
}

// Improved: reverse all, then reverse the first k and the rest. T: O(n), S: O(1).
void rotate(vector<int>& nums, int k) {
    int n = nums.size();
    // rotating by n is a no-op
    k %= n;
    reverse(nums.begin(), nums.end());
    reverse(nums.begin(), nums.begin() + k);
    reverse(nums.begin() + k, nums.end());
}

void test(const string& label, const vector<int>& nums, int k, const vector<int>& want) {
    vector<int> copied = nums;
    vector<int> reversed = nums;
    rotateCopy(copied, k);
    rotate(reversed, k);
    bool ok = copied == want && reversed == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {1, 2, 3, 4}, 1, {4, 1, 2, 3});
    test("big-k", {1, 2, 3}, 4, {3, 1, 2});
    test("zero", {1, 2}, 0, {1, 2});
}

// follow-up: rotate left by k -> rotate right by n - k % n, or reverse the first k,
//   the rest, then all
