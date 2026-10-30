// LC 4066: you may do at most one operation: pick distinct values x and y and replace
// every x with y. Return the max number of adjacent equal pairs afterwards.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::max;
using std::min;

// Replacing x by y keeps every equal pair and makes exactly the adjacent {x, y} pairs
// equal, so answer = existing equal pairs + the most common unequal adjacent pair
// (unordered). T: O(n log n), S: O(n).
int maxEqualPairs(const vector<int>& nums) {
    int equal = 0, gain = 0;
    // pairs[{a, b}] = adjacent positions holding a and b (a < b), in either order
    map<pair<int, int>, int> pairs;
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        if (nums[i] == nums[i - 1]) {
            equal++;
            continue;
        }
        pair<int, int> key = {min(nums[i], nums[i - 1]), max(nums[i], nums[i - 1])};
        pairs[key]++;
        gain = max(gain, pairs[key]);
    }
    return equal + gain;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxEqualPairs(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("both-sides", {5, 6, 5}, 2);
    test("keep-equal", {7, 7, 8}, 2);
    test("pair", {1, 2}, 1);
    test("same", {3, 3}, 1);
    test("best-pair", {1, 2, 1, 3, 1, 3}, 3);
}
