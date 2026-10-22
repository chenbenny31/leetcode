// LC 45 Jump Game II: nums[i] is the max jump length from index i. Return the minimum
// number of jumps to reach the last index (it is always reachable).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Implicit BFS: each jump is one level; the level ends at curEnd, the farthest index
// reachable with the jumps so far. T: O(n), S: O(1).
int jump(const vector<int>& nums) {
    int n = nums.size();
    int jumps = 0;
    int curEnd = 0;   // last index reachable with `jumps` jumps
    int far = 0;      // farthest index reachable with `jumps + 1` jumps
    // stop before the last index: landing on it needs no further jump
    for (int i = 0; i + 1 < n; i++) {
        far = max(far, i + nums[i]);
        if (i == curEnd) {
            jumps++;
            curEnd = far;
        }
    }
    return jumps;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (jump(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-jumps", {1, 3, 1, 1, 1}, 2);
    test("one-jump", {3, 0, 0, 0}, 1);
    test("single", {0}, 0);
}
