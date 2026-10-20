// nums[i] = max jump len from index i, start at index 0
// return whether the last index is reachable
// follow-up (jump game ii): last index always reachable, return the min num of jumps

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// greedy, track the farthest reachable index, T: O(n), S: O(1)
bool canJump(const vector<int>& nums) {
    int far = 0; // farthest index reachable from nums[0..i)
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        if (i > far) { return false; }
        far = max(far, i + nums[i]);
    }
    return true;
}

// follow-up: implicit BFS, one layer = indices reachable with the same num of jumps
// T: O(n), S: O(1)
int jump(const vector<int>& nums) {
    int jumps = 0;
    int end = 0; // last index reachable with `jumps` jumps
    int far = 0; // farthest index reachable with jumps + 1
    int n = nums.size();
    // stop before the last index, standing on it needs no more jump
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
    if (canJump(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("reach", {2, 0, 1}, true);
    test("stuck", {1, 0, 1}, false);
    test("one", {0}, true);
}
