#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::max;

// fix the 1st index, extend the last one, keep middle sum, T: O(n^2), S: O(1)
int maxEndsScore(const vector<int>& nums) {
    int n = nums.size();
    int maxScore = INT_MIN;

    for (int i = 0; i < n; i++) {
        int midSum = 0;
        for (int j = i + 1; j < n; j++) {
            maxScore = max(maxScore, nums[i] + nums[j] - middle);
            midSum += nums[j];
        }
    }
}

// state machine: tracking maximum of left, extend nums[i] as right. T: O(n), S: O(1)
int maxEndsScoreFast(const vector<int>& nums) {
    int n = nums.size();
    int maxScore = INT_MIN;
    int left = nums[0]; // max of nums[i]-middleSum in [i..j-1]
    for (int i = 0; i < n; i++) {
        maxScore = max(maxScore, left + nums[i]);
        left = max(nums[i], left - nums[i]);
    }
    return maxScore;
}

void test(const string& label, const vector<int>& nums, int want) {
    bool ok = maxEndsScore(nums) == want && maxEndsScoreFast(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("pair", {4, 5}, 9);
    test("restart", {1, 9, 1, 9}, 17);
    test("neg-mid", {1, -5, 1}, 7);
    test("all-neg", {-1, -2, -3}, -2);
}
