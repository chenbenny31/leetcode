// LC 377 Combination Sum IV: nums are distinct positive ints. Count the sequences
// (order matters) of nums, each reusable, that sum to target; the answer fits in int.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Unbounded knapsack counting permutations: target outer, items inner, so every last
// item choice is counted. unsigned: unreachable-target sums may wrap, and the answer
// itself fits. T: O(target * n), S: O(target).
int combinationSum4(const vector<int>& nums, int target) {
    vector<unsigned> ways(target + 1, 0);   // ways[t] = sequences summing to t
    ways[0] = 1;
    for (int t = 1; t <= target; t++) {
        for (int x : nums) {
            if (x <= t) { ways[t] += ways[t - x]; }
        }
    }
    return ways[target];
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    cout << label << (combinationSum4(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("order-matters", {1, 2}, 3, 3);
    test("exact-one", {5}, 5, 1);
    test("unreachable", {3}, 2, 0);
}
