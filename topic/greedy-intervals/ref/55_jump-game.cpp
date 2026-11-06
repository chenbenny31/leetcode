// LC 55 Jump Game: nums[i] is the max jump from index i; can you reach the last index
// from index 0?
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Greedy: track the farthest reachable index; fail if i is beyond it.
// T: O(n), S: O(1).
bool canJump(const vector<int>& nums) {
    int n = nums.size();
    int far = 0;   // farthest index reachable from indices scanned so far
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
    test("reachable", {2, 3, 0, 0, 4}, true);
    test("stuck-on-zero", {1, 0, 1}, false);
    test("single", {0}, true);
}
