// LC 55 Jump Game (Medium). Pattern: greedy farthest reach (LC 45: implicit BFS layers).
// nums[i] = max jump length from index i; starting at index 0, can you reach the last
// index? LC 45: the last index is always reachable; return the min number of jumps.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Track the farthest reachable index; fail once i lies beyond it. T: O(n), S: O(1).
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

// LC 45: implicit BFS; one layer = indices reachable with the same jump count.
// T: O(n), S: O(1).
int jump(const vector<int>& nums) {
    int n = nums.size();
    int jumps = 0;
    // last index reachable with `jumps` jumps
    int end = 0;
    // farthest index reachable with jumps + 1
    int far = 0;
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
    test("over-zero", {2, 0, 0}, true);
    test("stuck-on-zero", {1, 0, 1}, false);
    test("single", {0}, true);
    testJump("jump-skip-short", {1, 3, 1, 1, 1}, 2);
    testJump("jump-one-hop", {4, 1, 1, 1, 1}, 1);
    testJump("jump-single", {0}, 0);
}

// follow-up: jump +/- arr[i], reach any 0 (LC 1306) -> BFS/DFS with visited, O(n)
// follow-up: return the jump path (LC 45) -> remember the i that set far for each layer
