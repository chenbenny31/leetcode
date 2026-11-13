// LC 55 Jump Game (Medium) — greedy farthest reach
// nums[i] = max jump len from idx i; start at idx 0, return if can reach last idx
//
// clarify:
// nums[i] is a max -> shorter jumps ok
// values >= 0 -> never jump back
// single element -> true
// empty input -> ask; here true

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// core idea: track farthest idx reachable so far; fail once i > far
// T: O(n), S: O(1)
bool canJump(const vector<int>& nums) {
    int n = nums.size();
    // farthest idx reachable from nums[0..i)
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

// follow-up:
// min num of jumps (LC 45) -> BFS layers: at i == end do jumps++, end = far
// return the jump path (LC 45) -> remember the i that set far for each layer
// jump +/- arr[i], reach any 0 (LC 1306) -> BFS / DFS w/ visited, O(n)
