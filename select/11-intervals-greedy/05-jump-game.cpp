// LC 55 Jump Game (Medium) — greedy farthest reach
// nums[i] = max jump length from index i; starting at index 0, can you reach the last
// index?
// Clarify: nums[i] is a max, shorter jumps allowed; values non-negative; single
//   element is true; empty input.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Core idea: track the farthest index reachable so far; fail once i lies beyond it.
// T: O(n), S: O(1).
bool canJump(const vector<int>& nums) {
    int n = nums.size();
    // farthest index reachable from nums[0..i)
    int far = 0;
    for (int i = 0; i < n; i++) {
        if (i > far) { return false; }
        far = max(far, i + nums[i]);
    }
    return true;
}

void test(const string& label, const vector<int>& nums, bool want) {
    cout << label << (canJump(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("over-zero", {2, 0, 0}, true);
    test("stuck-on-zero", {1, 0, 1}, false);
    test("single", {0}, true);
}

// follow-up (ideas only):
// - min number of jumps (LC 45) -> implicit BFS layers: end = last index of the current
//   layer, far = reach of the next; at i == end do jumps++, end = far; stop at n - 2
// - return the jump path (LC 45) -> remember the i that set far for each layer
// - jump +/- arr[i], reach any 0 (LC 1306) -> BFS / DFS with a visited array, O(n)
