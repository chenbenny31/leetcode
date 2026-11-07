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

// Move everything to the median: shifting the target off it by 1 costs a move for each
// element on one side and saves one for each on the other, and the far side is never
// smaller. T: O(n log n), S: O(n) for the sorted copy.
long long minMoves2(vector<int> nums) {
    int n = nums.size();
    sort(nums.begin(), nums.end());
    int median = nums[n / 2];

    long long moves = 0;
    for (int x : nums) { moves += abs(1LL * x - median); }
    return moves;
}

void test(const string& label, const vector<int>& nums, long long want) {
    cout << label << (minMoves2(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("outlier", {1, 2, 100}, 99);
    test("even-count", {1, 2, 3, 4}, 4);
    test("negatives", {-5, 0, 5}, 10);
    test("single", {7}, 0);
}
