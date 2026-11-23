// 47 · Predict The Winner (Cisco LC tag, Medium) — LC 486, interval DP (minimax)
// two players take turns removing a num from either end and add it to their score;
// both play optimally. Return true if player 1 wins or ties
//
// clarify:
// tie -> player 1 wins (LC)
// values -> non-negative ints, n <= 20 on LC (the DP works for any n)
// return who wins, not the scores -> yes (scores are a follow-up)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// core idea: on nums[i..j] the player to move maximizes (own score - other's score);
// taking an end leaves the other player the same problem on a shorter range
// T: O(n^2), S: O(n^2)
bool predictTheWinner(const vector<int>& nums) {
    int n = nums.size();
    // diff[i][j] = best (mover - opponent) on nums[i..j]
    vector<vector<int>> diff(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) { diff[i][i] = nums[i]; }
    // shorter ranges first, so diff[i + 1][j] and diff[i][j - 1] are ready
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            diff[i][j] = max(nums[i] - diff[i + 1][j], nums[j] - diff[i][j - 1]);
        }
    }
    return diff[0][n - 1] >= 0;
}

void test(const string& label, const vector<int>& nums, bool want) {
    cout << label << (predictTheWinner(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("loses", {1, 5, 2}, false);
    test("wins", {1, 5, 233, 7}, true);
    test("single", {5}, true);
    test("tie", {1, 1}, true);
    return 0;
}

// follow-up:
// 1. O(n) space -> keep one row: diff[j] over i descending (diff[i][j] needs row i + 1)
// 2. actual scores -> p1 = (total + diff) / 2, p2 = (total - diff) / 2
// 3. LC 877 stone game (even n) -> player 1 always wins: all odd or all even idx
// 4. top-down -> f(i, j) = max(nums[i] - f(i+1, j), nums[j] - f(i, j-1)) + memo
