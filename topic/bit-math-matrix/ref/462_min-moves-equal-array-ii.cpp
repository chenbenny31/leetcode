// LC 462 Minimum Moves to Equal Array Elements II: fewest +1/-1 moves to make all equal.
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::abs;
using std::sort;
using std::nth_element;

// Move everything to the median: shifting the target off it by 1 costs a move for each
// element on one side and saves one for each on the other, and the far side is never
// smaller. T: O(n log n), S: O(1).
long long minMoves2(vector<int> nums) {
    int n = nums.size();
    sort(nums.begin(), nums.end());
    int median = nums[n / 2];

    long long moves = 0;
    for (int x : nums) { moves += abs(1LL * x - median); }
    return moves;
}

// Quickselect the median instead of sorting. T: O(n) average, S: O(1).
long long minMoves2Select(vector<int> nums) {
    int n = nums.size();
    nth_element(nums.begin(), nums.begin() + n / 2, nums.end());
    int median = nums[n / 2];

    long long moves = 0;
    for (int x : nums) { moves += abs(1LL * x - median); }
    return moves;
}

void test(const string& label, const vector<int>& nums, long long want) {
    bool ok = minMoves2(nums) == want && minMoves2Select(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("outlier", {1, 2, 100}, 99);
    test("even-count", {1, 2, 3, 4}, 4);
    test("negatives", {-5, 0, 5}, 10);
    test("single", {7}, 0);
}
