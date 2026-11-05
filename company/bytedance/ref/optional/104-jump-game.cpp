// ByteDance / LC 55: nums[i] = max jump length from index i; starting at index 0, can
// you reach the last index?
// Follow-up LC 45: the last index is always reachable; return the min number of jumps.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Greedy: track the farthest reachable index. T: O(n), S: O(1).
bool canJump(const vector<int>& nums) {
    // farthest index reachable from nums[0..i)
    int far = 0;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // i lies past every reachable index
        if (i > far) { return false; }
        far = max(far, i + nums[i]);
    }
    return true;
}

// follow-up: implicit BFS; one layer = indices reachable with the same jump count.
// T: O(n), S: O(1).
int jump(const vector<int>& nums) {
    int jumps = 0;
    // last index reachable with `jumps` jumps
    int end = 0;
    // farthest index reachable with jumps + 1
    int far = 0;
    int n = nums.size();
    // stop before the last index: standing on it needs no further jump
    for (int i = 0; i < n - 1; i++) {
        far = max(far, i + nums[i]);
        if (i == end) {
            jumps++;
            end = far;
        }
    }
    return jumps;
}

void test(const string& label, const vector<int>& nums, bool want) {
    cout << label << (canJump(nums) == want ? " PASS\n" : " FAIL\n");
}

void testJump(const string& label, const vector<int>& nums, int want) {
    cout << label << (jump(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {2, 3, 1, 1, 4}, true);
    test("stuck-at-0", {3, 2, 1, 0, 4}, false);
    test("single", {0}, true);
    testJump("jump-lc-ex1", {2, 3, 1, 1, 4}, 2);
    testJump("jump-single", {0}, 0);
}

// follow-up: min jumps (LC 45) -> see jump() above; implicit BFS, jumps++ when i hits end
// follow-up: jump +/- arr[i], reach a 0 (LC 1306) -> BFS/DFS with visited, O(n)
// follow-up: max score with jumps <= k (LC 1696) -> dp[i] = nums[i] + max(dp[i-k..i-1]),
//   monotonic deque of indices for the window max, O(n)
