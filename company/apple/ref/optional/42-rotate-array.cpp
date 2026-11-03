// LC 189: rotate nums to the right by k steps in place (k may exceed n).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::reverse;

// straightforward: element i moves to (i + k) % n in a copy. T: O(n), S: O(n).
void rotateCopy(vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> result(n);   // result[(i + k) % n] = nums[i]
    for (int i = 0; i < n; i++) {
        result[(i + k) % n] = nums[i];
    }
    nums = result;
}

// improved: reverse all, then reverse the first k and the rest. T: O(n), S: O(1).
void rotate(vector<int>& nums, int k) {
    int n = nums.size();
    if (n == 0) { return; }
    // rotating by n is a no-op
    k %= n;
    reverse(nums.begin(), nums.end());
    reverse(nums.begin(), nums.begin() + k);
    reverse(nums.begin() + k, nums.end());
}

void test(const string& label, const vector<int>& nums, int k, const vector<int>& want) {
    vector<int> a = nums, b = nums;
    rotateCopy(a, k);
    rotate(b, k);
    cout << label << (a == want && b == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {1, 2, 3, 4}, 1, {4, 1, 2, 3});
    test("big-k", {1, 2, 3}, 4, {3, 1, 2});
    test("zero", {1, 2}, 0, {1, 2});
}
