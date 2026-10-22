// LC 174 Dungeon Game: a knight starts at the top-left of a grid and must reach the
// bottom-right moving right or down; each cell adds or removes health. Health must
// stay >= 1 at all times. Return the minimum starting health.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using std::cout;
using std::max;
using std::min;
using std::string;
using std::vector;

// Backward dp from the princess: health needed on entering a cell. A forward dp
// cannot decide, since the path needs both low prefix sum and high final sum.
// T: O(m*n), S: O(n).
int calculateMinimumHP(const vector<vector<int>>& dungeon) {
    int m = dungeon.size(), n = dungeon[0].size();
    // dp[j] = min health needed on entering column j of the current row (dp[n] is
    // a wall); dp[n-1] = 1 seeds the bottom-right cell with "need 1 after it"
    vector<int> dp(n + 1, INT_MAX);
    dp[n - 1] = 1;
    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            int need = min(dp[j], dp[j + 1]) - dungeon[i][j];
            dp[j] = max(1, need);
        }
    }
    return dp[0];
}

void test(const string& label, const vector<vector<int>>& dungeon, int want) {
    cout << label << (calculateMinimumHP(dungeon) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("trap", {{-3}}, 4);
    test("heal", {{5}}, 1);
    test("two-by-two", {{1, -3}, {-2, 5}}, 2);
    test("corridor", {{-2, -3, 3}}, 6);
}
